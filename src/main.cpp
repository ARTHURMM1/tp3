#include <iostream>
#include <iomanip>
#include <string>
#include "Log.hpp"
#include "Escalonador.hpp"
#include "Pacote.hpp"
#include "Armazem.hpp"

// Função utilitária para encontrar o índice da seção de destino em um armazém
int encontrarIndiceSecao(Armazem* armazem, int idDestino) {
    if (!armazem) return -1;
    int idx = 0;
    List<PilhaPorDestino*>::Node* curr = armazem->secoes.get_head();
    while (curr) {
        if (curr->data->idDestino == idDestino) return idx;
        curr = curr->next;
        idx++;
    }
    return -1;
}

int main() {
    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao, numeroArmazens;
    std::cin >> capacidadeTransporte >> latenciaTransporte >> intervaloTransportes >> custoRemocao >> numeroArmazens;

    Log logistica;
    logistica.setNumArmazens(numeroArmazens);
    Escalonador escalonador;

    // Cria armazéns
    for (int i = 0; i < numeroArmazens; i++) {
        Armazem* armazem = new Armazem(i);
        logistica.add_storage(armazem);
    }

    // Lê matriz de adjacência e cria conexões
    for (int i = 0; i < numeroArmazens; i++) {
        for (int j = 0; j < numeroArmazens; j++) {
            int conexao;
            std::cin >> conexao;
            if (conexao == 1) {
                logistica.add_path(i, j);
            }
        }
    }

    // Lê pacotes e agenda eventos de chegada
    int numeroPacotes;
    std::cin >> numeroPacotes;
    for (int i = 0; i < numeroPacotes; i++) {
        int tempoChegada, idPacote, origem, destino;
        std::string dummy;
        std::cin >> tempoChegada >> dummy >> idPacote >> dummy >> origem >> dummy >> destino;

        Pacote* pacote = new Pacote(idPacote, origem, destino, tempoChegada);
        logistica.calcularRota(origem, destino, pacote->rota);

        pacote->estado = CHEGADA_ESCALONADA;
        Evento eventoChegada(tempoChegada, CHEGADA_PACOTE, pacote);
        eventoChegada.idArmazemOrigem = origem;
        escalonador.insereEvento(eventoChegada);
    }

    // Agenda eventos de transporte periódicos para cada par origem-destino
    for (int origem = 0; origem < numeroArmazens; origem++) {
        for (int destino = 0; destino < numeroArmazens; destino++) {
            if (origem != destino) {
                // Verifica se existe conexão direta
                List<VerticeArmazem*>::Node* curr = logistica.encontrarVertice(origem)->vizinhos.get_head();
                bool temConexao = false;
                while (curr) {
                    if (curr->data->storage->id == destino) {
                        temConexao = true;
                        break;
                    }
                    curr = curr->next;
                }
                if (temConexao) {
                    escalonador.insereEvento(Evento(intervaloTransportes, TRANSPORTE_PACOTES, origem, destino));
                }
            }
        }
    }

    // Loop principal da simulação
    while (!escalonador.is_empty()) {
        Evento evento = escalonador.retiraProximoEvento();

        if (evento.tipo == CHEGADA_PACOTE) {
            Pacote* pacote = evento.pacote;
            if (!pacote || pacote->estado == ENTREGUE) continue;

            // Se chegou ao destino final, entrega e deleta
            if (pacote->idArmazemOrigem == pacote->idArmazemDestino) {
                evento.imprimirEvento("entregue", pacote->idArmazemDestino);
                pacote->estado = ENTREGUE;
                continue;
            }

            Armazem* armazem = logistica.encontrarArmazem(pacote->idArmazemOrigem);
            if (!armazem) {
                delete pacote;
                continue;
            }

            int proximoDestino = pacote->getProximoDestino();
            armazem->armazenarPacote(pacote, proximoDestino);
            int indiceSecao = encontrarIndiceSecao(armazem, proximoDestino);
            evento.imprimirEvento("armazenado", pacote->idArmazemOrigem, -1, indiceSecao);

            int tempoTransporte = evento.tempo;
            if (tempoTransporte % intervaloTransportes != 0)
                tempoTransporte += intervaloTransportes - (tempoTransporte % intervaloTransportes);

            escalonador.insereEvento(Evento(tempoTransporte, TRANSPORTE_PACOTES, pacote->idArmazemOrigem, proximoDestino));
        } 
        else if (evento.tipo == TRANSPORTE_PACOTES) {
            Armazem* origem = logistica.encontrarArmazem(evento.idArmazemOrigem);

            if (origem) {
                for (int i = 0; i < capacidadeTransporte; i++) {
                    Pacote* pacote = origem->retirarPacote(evento.idArmazemDestino);
                    if (!pacote || pacote->estado == ENTREGUE) {
                        break;
                    }

                    int indiceSecao = encontrarIndiceSecao(origem, evento.idArmazemDestino);

                    evento.pacote = pacote;
                    evento.imprimirEvento("removido", evento.idArmazemOrigem, -1, indiceSecao);
                    evento.imprimirEvento("transito", evento.idArmazemOrigem, evento.idArmazemDestino);

                    pacote->idArmazemOrigem = evento.idArmazemDestino;
                    pacote->avancarRota();

                    // Só agenda chegada se não chegou ao destino final
                    if (pacote->idArmazemOrigem != pacote->idArmazemDestino) {
                        Evento eventoChegada(evento.tempo + latenciaTransporte, CHEGADA_PACOTE, pacote);
                        eventoChegada.idArmazemOrigem = evento.idArmazemDestino;
                        escalonador.insereEvento(eventoChegada);
                    }
                }

                if (intervaloTransportes > 0) {
                    escalonador.insereEvento(Evento(evento.tempo + intervaloTransportes, TRANSPORTE_PACOTES, evento.idArmazemOrigem, evento.idArmazemDestino));
                }
            }
        }
    }

    return 0;
}