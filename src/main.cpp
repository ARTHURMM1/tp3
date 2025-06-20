#include <iostream>
#include <iomanip>
#include <string>
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


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao, numeroArmazens;
    std::cin >> capacidadeTransporte >> latenciaTransporte >> intervaloTransportes >> custoRemocao >> numeroArmazens;

    Log logistica;
    logistica.setNumArmazens(numeroArmazens);
    Escalonador escalonador;

    for (int i = 0; i < numeroArmazens; i++) {
        logistica.add_storage(new Armazem(i));
    }

    for (int i = 0; i < numeroArmazens; i++) {
        for (int j = 0; j < numeroArmazens; j++) {
            int conexao;
            std::cin >> conexao;
            if (conexao == 1) {
                logistica.add_path(i, j);
            }
        }
    }

    int numeroPacotes;
    std::cin >> numeroPacotes;
    
    int pacotesAtivos = numeroPacotes;

    for (int i = 0; i < numeroPacotes; i++) {
        int tempoChegada, idPacote, origem, destino;
        std::string dummy;
        std::cin >> tempoChegada >> dummy >> idPacote >> dummy >> origem >> dummy >> destino;
        Pacote* pacote = new Pacote(idPacote, origem, destino, tempoChegada);
        logistica.calcularRota(origem, destino, pacote->rota);
        if(!pacote->rota.is_empty()) pacote->rota.pop_front(); 
        pacote->estado = CHEGADA_ESCALONADA;
        escalonador.insereEvento(Evento(tempoChegada, EVENTO_PACOTE, pacote));
    }

    for (int i = 0; i < numeroArmazens; i++) {
        VerticeArmazem* v = logistica.encontrarVertice(i);
        if(v) {
            auto curr = v->vizinhos.get_head();
            while(curr) {
                int j = curr->data->storage->id;
                escalonador.insereEvento(Evento(intervaloTransportes, EVENTO_TRANSPORTE, i, j));
                curr = curr->next;
            }
        }
    }

    while (!escalonador.is_empty()) {
        Evento evento = escalonador.retiraProximoEvento();

        if (evento.tipo == EVENTO_PACOTE) {
            Pacote* pacote = evento.pacote;
            int armazemAtualId = pacote->idArmazemOrigem;

            if (armazemAtualId == pacote->idArmazemDestino) {
                pacote->estado = ENTREGUE;
                Evento evtLog(evento.tempo, EVENTO_PACOTE, pacote);
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
            int idOrigem = evento.idArmazemOrigem;
            int idDestino = evento.idArmazemDestino;
            
            Armazem* armazemOrigem = logistica.encontrarArmazem(idOrigem);
            if (armazemOrigem) {
                Stack<Pacote*>* pilha = armazemOrigem->getPilhaPorDestino(idDestino);

                if (pilha && !pilha->is_empty()) {
                    List<Pacote*> pacotes_na_secao;
                    while (!pilha->is_empty()) {
                        pacotes_na_secao.push_front(pilha->pop());
                    }

                    List<Pacote*> para_transportar;
                    for (int i = 0; i < capacidadeTransporte; ++i) {
                        Pacote* mais_antigo = nullptr;
                        auto node_iter = pacotes_na_secao.get_head();
                        while(node_iter) {
                            Pacote* candidato = node_iter->data;
                            if (!pacoteEstaNaLista(candidato, para_transportar)) {
                                if (mais_antigo == nullptr || candidato->tempoPostagem < mais_antigo->tempoPostagem) {
                                    mais_antigo = candidato;
                                }
                            }
                            node_iter = node_iter->next;
                        }
                        if (mais_antigo != nullptr) {
                            para_transportar.push_back(mais_antigo);
                        } else {
                            break; // Não há mais pacotes para selecionar
                        }
                    }

                    double tempo_log = evento.tempo;
                    auto node_iter = pacotes_na_secao.get_head();
                    while (node_iter) {
                        tempo_log += custoRemocao;
                        Evento logRemocao(tempo_log, EVENTO_TRANSPORTE, idOrigem, idDestino);
                        logRemocao.pacote = node_iter->data;
                        logRemocao.imprimir("removido de");
                        node_iter = node_iter->next;
                    }

                    Stack<Pacote*> pilha_rearmazenar;
                    node_iter = pacotes_na_secao.get_head();
                    while(node_iter) {
                        Pacote* p = node_iter->data;
                        if (pacoteEstaNaLista(p, para_transportar)) {
                            Evento logTransito(tempo_log, EVENTO_TRANSPORTE, idOrigem, idDestino);
                            logTransito.pacote = p;
                            logTransito.imprimir("em transito de");
                            
                            p->avancarRota();
                            p->idArmazemOrigem = idDestino;
                            p->estado = CHEGADA_ESCALONADA;
                            escalonador.insereEvento(Evento(tempo_log + latenciaTransporte, EVENTO_PACOTE, p));
                        } else {
                            pilha_rearmazenar.push(p);
                        }
                        node_iter = node_iter->next;
                    }
                    
                    while(!pilha_rearmazenar.is_empty()) {
                        Pacote* p = pilha_rearmazenar.pop();
                        Evento logRearmazena(tempo_log, EVENTO_TRANSPORTE, idOrigem, idDestino);
                        logRearmazena.pacote = p;
                        logRearmazena.imprimir("rearmazenado em");
                        pilha->push(p);
                    }
                }
            }
            
            if (intervaloTransportes > 0 && pacotesAtivos > 0) {
                escalonador.insereEvento(Evento(evento.tempo + intervaloTransportes, EVENTO_TRANSPORTE, idOrigem, idDestino));
            }
        }
    }

    return 0;
}