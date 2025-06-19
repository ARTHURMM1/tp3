#include <iostream>
#include <string>
#include "Log.hpp"
#include "Escalonador.hpp"
#include "Evento.hpp"
#include "Pacote.hpp"

// Constantes da simulação, conforme mencionado no PDF 
const double TEMPO_TRANSPORTE = 4.0;      // 4 horas de transporte entre armazéns
const double TEMPO_DIARIO_TRANSPORTE = 24.0; // Eventos de transporte ocorrem a cada 24h
const int CAPACIDADE_TRANSPORTE = 5;      // 5 pacotes por transporte

int main() {
    Log logistica;
    Escalonador escalonador;
    double tempoSimulacao = 0.0;
    int contadorPacotes = 0;

    // --- FASE DE CONFIGURAÇÃO ---

    // 1. Ler topologia da rede (exemplo hard-coded)
    int numArmazens = 5;
    Armazem armazens[numArmazens];
    for (int i = 0; i < numArmazens; ++i) {
        armazens[i] = Armazem(i);
        logistica.add_storage(&armazens[i]);
    }
    
    // Conexões 
    logistica.add_path(0, 1);
    logistica.add_path(1, 2);
    logistica.add_path(1, 3);
    logistica.add_path(2, 4);
    logistica.add_path(3, 4);
    
    // 2. Agendar eventos de transporte iniciais para cada ligação 
    for (int i = 0; i < numArmazens; i++) {
        Armazem* armazem = logistica.encontrarArmazem(i);
        if (armazem) {
            List<VerticeArmazem*>::Node* vizinhoNode = armazem->vizinhos.get_head();
            while(vizinhoNode) {
                int idVizinho = vizinhoNode->data->storage.getId();
                if (i < idVizinho) {
                    escalonador.insereEvento(Evento(
                        TEMPO_DIARIO_TRANSPORTE, 
                        TRANSPORTE_PACOTES, 
                        i, 
                        idVizinho
                    ));
                }
                vizinhoNode = vizinhoNode->next;
            }
        }
    }
    
    // 3. Ler pacotes e agendar chegadas iniciais (exemplo hard-coded) 
    Pacote* p1 = new Pacote(++contadorPacotes, 0, 4, 1.0);
    p1->rota = logistica.calcularRota(0, 4);
    escalonador.insereEvento(Evento(p1->tempoPostagem, CHEGADA_PACOTE, p1));

    Pacote* p2 = new Pacote(++contadorPacotes, 0, 2, 2.5);
    p2->rota = logistica.calcularRota(0, 2);
    escalonador.insereEvento(Evento(p2->tempoPostagem, CHEGADA_PACOTE, p2));

    std::cout << "Simulacao iniciada." << std::endl;

    // --- FASE DE SIMULAÇÃO ---
    // Loop principal da simulação 
    while (!escalonador.is_empty()) {
        Evento eventoAtual = escalonador.retiraProximoEvento();
        tempoSimulacao = eventoAtual.tempo;

        if (eventoAtual.tipo == CHEGADA_PACOTE) {
            Pacote* pacote = eventoAtual.pacote;
            int idArmazemAtual = pacote->estado == NAO_POSTADO ? 
                pacote->idArmazemOrigem : 
                (!pacote->rota.is_empty() ? pacote->rota.front() : -1);
            
            if (idArmazemAtual == -1) {
                std::cout << "Erro: rota vazia para pacote " << pacote->id << std::endl;
                continue;
            }

            if (pacote->estado != NAO_POSTADO) {
                pacote->avancarRota();
            }

            Armazem* armazemAtual = logistica.encontrarArmazem(idArmazemAtual);

            // Calcula estatísticas de tempo em trânsito
            if(pacote->estado == ALOCADO_TRANSPORTE) {
                // ... lógica para estatísticas ...
            }

            std::cout << "T=" << tempoSimulacao << ": Pacote " << pacote->id << " chegou no armazem " << armazemAtual->getId() << std::endl;

            // Verifica se chegou ao destino final 
            if (armazemAtual->getId() == pacote->idArmazemDestino) {
                pacote->estado = ENTREGUE;
                std::cout << "T=" << tempoSimulacao << ": Pacote " << pacote->id << " entregue no destino final!" << std::endl;
                delete pacote; // Libera memória
            } else {
                // Armazena o pacote na seção correta
                int proximoDestino = pacote->getProximoDestino();
                armazemAtual->armazenarPacote(pacote, proximoDestino);
                pacote->estado = ARMAZENADO;
                pacote->tempoEntradaArmazem = tempoSimulacao; // Registra o tempo de armazenamento 
                std::cout << "T=" << tempoSimulacao << ": Pacote " << pacote->id << " armazenado na secao para o armazem " << proximoDestino << std::endl;
            }

        } else if (eventoAtual.tipo == TRANSPORTE_PACOTES) {
            int idOrigem = eventoAtual.idArmazemOrigem;
            int idDestino = eventoAtual.idArmazemDestino;
            Armazem* armazemOrigem = logistica.encontrarArmazem(idOrigem);
            
            std::cout << "T=" << tempoSimulacao << ": Evento de transporte do armazem " << idOrigem << " para " << idDestino << std::endl;

            // Remove pacotes da seção correspondente até a capacidade máxima 
            for (int i = 0; i < CAPACIDADE_TRANSPORTE; ++i) {
                Pacote* pacote = armazemOrigem->retirarPacote(idDestino);
                if (pacote) {
                    // Atualiza estatísticas de armazenamento
                    pacote->tempoTotalArmazenado += tempoSimulacao - pacote->tempoEntradaArmazem;
                    
                    pacote->estado = ALOCADO_TRANSPORTE;
                    
                    // Agenda a chegada no próximo armazém
                    escalonador.insereEvento(Evento(tempoSimulacao + TEMPO_TRANSPORTE, CHEGADA_PACOTE, pacote));
                     std::cout << "T=" << tempoSimulacao << ": Pacote " << pacote->id << " em transito para o armazem " << idDestino << std::endl;
                } else {
                    break; // Não há mais pacotes nesta seção
                }
            }
            
            // Reagenda o próximo evento de transporte para esta rota 
            escalonador.insereEvento(Evento(tempoSimulacao + TEMPO_DIARIO_TRANSPORTE, TRANSPORTE_PACOTES, idOrigem, idDestino));
        }
    }

    std::cout << "Simulacao finalizada." << std::endl;

    return 0;
}