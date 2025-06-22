#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "Log.hpp"
#include "Escalonador.hpp"
#include "Pacote.hpp"
#include "Armazem.hpp"
#include "List.hpp"

// Função auxiliar para verificar se um pacote está numa lista
bool pacoteEstaNaLista(Pacote* p, List<Pacote*>& lista) {
    auto node = lista.get_head();
    while(node) {
        if (node->data->id == p->id) {
            return true;
        }
        node = node->next;
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << argv[1] << std::endl;
        return 1;
    }

    std::ios_base::sync_with_stdio(false);
    std::cout.tie(NULL);

    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao, numeroArmazens;
    inputFile >> capacidadeTransporte >> latenciaTransporte >> intervaloTransportes >> custoRemocao >> numeroArmazens;

    Log logistica;
    logistica.setNumArmazens(numeroArmazens);
    Escalonador escalonador;

    for (int i = 0; i < numeroArmazens; i++) {
        logistica.add_storage(new Armazem(i));
    }

    for (int i = 0; i < numeroArmazens; i++) {
        for (int j = 0; j < numeroArmazens; j++) {
            int conexao;
            inputFile >> conexao;
            if (conexao == 1) {
                logistica.add_path(i, j);
            }
        }
    }

    int numeroPacotes;
    inputFile >> numeroPacotes;
    
    int pacotesAtivos = numeroPacotes;
    int tempoPrimeiroPacote = -1;

    for (int i = 0; i < numeroPacotes; i++) {
        int tempoChegada, idPacoteDoArquivo, origem, destino;
        std::string dummy;
        inputFile >> tempoChegada >> dummy >> idPacoteDoArquivo >> dummy >> origem >> dummy >> destino;
        
        Pacote* pacote = new Pacote(idPacoteDoArquivo, i, origem, destino, tempoChegada); 
        
        logistica.calcularRota(origem, destino, pacote->rota);
        if(!pacote->rota.is_empty()) pacote->rota.pop_front(); 
        pacote->estado = CHEGADA_ESCALONADA;
        escalonador.insereEvento(Evento(tempoChegada, EVENTO_PACOTE, pacote));

        if (tempoPrimeiroPacote == -1 || tempoChegada < tempoPrimeiroPacote) {
            tempoPrimeiroPacote = tempoChegada;
        }
    }
    
    inputFile.close();
    
    if (numeroPacotes > 0) {
        double tempoPrimeiroTransporte = tempoPrimeiroPacote + intervaloTransportes + custoRemocao;
        for (int i = 0; i < numeroArmazens; i++) {
            VerticeArmazem* v = logistica.encontrarVertice(i);
            if(v) {
                auto curr = v->vizinhos.get_head();
                while(curr) {
                    int j = curr->data->storage->id;
                    escalonador.insereEvento(Evento(tempoPrimeiroTransporte, EVENTO_TRANSPORTE, i, j));
                    curr = curr->next;
                }
            }
        }
    }

    while (!escalonador.is_empty() && pacotesAtivos > 0) {
        Evento evento = escalonador.retiraProximoEvento();

        if (evento.tipo == EVENTO_PACOTE) {
            Pacote* pacote = evento.pacote;
            if (!pacote || pacote->estado == ENTREGUE) continue;

            int armazemAtualId = pacote->idArmazemOrigem;

            if (armazemAtualId == pacote->idArmazemDestino) {
                pacote->estado = ENTREGUE;
                Evento evtLog(evento.tempo, EVENTO_PACOTE, pacote);
                evtLog.idArmazemOrigem = armazemAtualId;
                evtLog.imprimir();
                delete pacote;
                pacotesAtivos--;
            } else {
                Armazem* armazemAtual = logistica.encontrarArmazem(armazemAtualId);
                int proximoDestino = pacote->getProximoDestino();
                armazemAtual->armazenarPacote(pacote, proximoDestino);
                pacote->estado = ARMAZENADO;
                
                Evento evtLog(evento.tempo, EVENTO_PACOTE, pacote);
                evtLog.idArmazemDestino = proximoDestino;
                evtLog.imprimir();
            }
        } else if (evento.tipo == EVENTO_TRANSPORTE) {
            if (intervaloTransportes > 0 && pacotesAtivos > 0) {
                escalonador.insereEvento(Evento(evento.tempo + intervaloTransportes, EVENTO_TRANSPORTE, evento.idArmazemOrigem, evento.idArmazemDestino));
            }

            if (pacotesAtivos == 0) continue;
            
            int idOrigem = evento.idArmazemOrigem;
            int idDestino = evento.idArmazemDestino;
            
            Armazem* armazemOrigem = logistica.encontrarArmazem(idOrigem);
            if (armazemOrigem) {
                Stack<Pacote*>* pilha = armazemOrigem->getPilhaPorDestino(idDestino);
                if (pilha && !pilha->is_empty()) {
                    List<Pacote*> pacotes_removidos_lifo;
                    while (!pilha->is_empty()) {
                        pacotes_removidos_lifo.push_back(pilha->pop());
                    }

                    List<Pacote*> para_transportar;
                    for (int i = 0; i < capacidadeTransporte; ++i) {
                        Pacote* mais_antigo = nullptr;
                        auto node_iter = pacotes_removidos_lifo.get_head();
                        while(node_iter) {
                            Pacote* candidato = node_iter->data;
                            if (!pacoteEstaNaLista(candidato, para_transportar)) {
                                if (mais_antigo == nullptr || 
                                    candidato->tempoPostagem < mais_antigo->tempoPostagem ||
                                    (candidato->tempoPostagem == mais_antigo->tempoPostagem && candidato->id < mais_antigo->id)) {
                                    mais_antigo = candidato;
                                }
                            }
                            node_iter = node_iter->next;
                        }
                        if (mais_antigo != nullptr) {
                            para_transportar.push_back(mais_antigo);
                        } else {
                            break; 
                        }
                    }
                    
                    double tempo_log = evento.tempo - custoRemocao;
                    auto node_iter = pacotes_removidos_lifo.get_head();
                    while (node_iter) {
                        tempo_log += custoRemocao;
                        Evento logRemocao(tempo_log, EVENTO_TRANSPORTE, idOrigem, idDestino);
                        logRemocao.pacote = node_iter->data;
                        logRemocao.imprimir("removido de");
                        node_iter = node_iter->next;
                    }
                    
                    double tempo_final = tempo_log;

                    // --- INÍCIO DA LÓGICA CORRETA E DEFINITIVA ---
                    
                    // 1. Reverte a lista de remoção para obter a ordem de chegada (FIFO)
                    List<Pacote*> pacotes_em_ordem_fifo;
                    node_iter = pacotes_removidos_lifo.get_head();
                    while(node_iter) {
                        pacotes_em_ordem_fifo.push_front(node_iter->data);
                        node_iter = node_iter->next;
                    }

                    // 2. Processa transporte e re-armazenamento iterando na ordem FIFO
                    Stack<Pacote*> pilha_rearmazenar;
                    node_iter = pacotes_em_ordem_fifo.get_head();
                    while(node_iter) {
                        Pacote* p = node_iter->data;
                        if (pacoteEstaNaLista(p, para_transportar)) {
                            // Imprime "em transito" na ordem FIFO
                            Evento logTransito(tempo_final, EVENTO_TRANSPORTE, idOrigem, idDestino);
                            logTransito.pacote = p;
                            logTransito.imprimir("em transito de");
                            p->avancarRota();
                            p->idArmazemOrigem = idDestino;
                            p->estado = CHEGADA_ESCALONADA;
                            escalonador.insereEvento(Evento(tempo_final + latenciaTransporte, EVENTO_PACOTE, p));
                        } else {
                            // Adiciona na pilha para re-armazenar, mantendo a ordem relativa
                            pilha_rearmazenar.push(p);
                        }
                        node_iter = node_iter->next;
                    }
                    
                    // 3. Empilha de volta na pilha principal na ordem LIFO correta
                    while(!pilha_rearmazenar.is_empty()){
                        Pacote* p_rearm = pilha_rearmazenar.pop();
                        Evento logRearmazena(tempo_final, EVENTO_TRANSPORTE, idOrigem, idDestino);
                        logRearmazena.pacote = p_rearm;
                        logRearmazena.imprimir("rearmazenado em");
                        pilha->push(p_rearm);
                    }
                    // --- FIM DA LÓGICA CORRETA E DEFINITIVA ---
                }
            }
        }
    }

    return 0;
}