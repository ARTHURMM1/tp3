#include <iostream>
#include <iomanip>
#include "Log.hpp"
#include "Escalonador.hpp"
#include "Pacote.hpp"

int main() {
    int K, T, C, N;
    std::cin >> K >> T >> C >> N;
    
    Log logistica;
    Escalonador escalonador;

    // Lê matriz de adjacência e cria armazéns
    for (int i = 0; i < N; i++) {
        Armazem* armazem = new Armazem(i);
        logistica.add_storage(armazem);
    }

    // Lê conexões da matriz
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int conexao;
            std::cin >> conexao;
            if (conexao == 1) {
                logistica.add_path(i, j);
            }
        }
    }

    // Lê pacotes
    int P;
    std::cin >> P;
    
    for (int i = 0; i < P; i++) {
        int tempo, id, origem, destino;
        std::string dummy;
        std::cin >> tempo >> dummy >> id >> dummy >> origem >> dummy >> destino;
        
        Pacote* pacote = new Pacote(id, origem, destino, tempo);
        pacote->estado = CHEGADA_ESCALONADA;
        
        // Calcula rota do pacote
        pacote->rota = logistica.calcularRota(origem, destino);
        
        // Agenda chegada do pacote
        Evento evento(tempo, CHEGADA_PACOTE, pacote);
        evento.idArmazemOrigem = origem;
        evento.idArmazemDestino = destino;
        escalonador.insereEvento(evento);
    }

    // Processa eventos
    while (!escalonador.is_empty()) {
        Evento evento = escalonador.retiraProximoEvento();
        
        if (evento.tipo == CHEGADA_PACOTE) {
            Pacote* pacote = evento.pacote;
            Armazem* armazem = logistica.encontrarArmazem(pacote->idArmazemOrigem);
            
            // Define próximo destino
            int proximoDestino = pacote->getProximoDestino();
            if (proximoDestino == -1) proximoDestino = pacote->idArmazemDestino;
            
            // Armazena pacote
            armazem->armazenarPacote(pacote, proximoDestino);
            
            // Imprime evento de armazenamento
            evento.idArmazemDestino = proximoDestino;
            evento.imprimirEvento("armazenado");
            
            // Agenda próximo transporte
            double tempoTransporte = evento.tempo;
            while ((int)tempoTransporte % K != 0) tempoTransporte++;
            
            if (tempoTransporte <= T) {
                Evento eventoTransporte(tempoTransporte, TRANSPORTE_PACOTES, 
                                      pacote->idArmazemOrigem, proximoDestino);
                eventoTransporte.pacote = pacote;
                escalonador.insereEvento(eventoTransporte);
            }
        }
        else if (evento.tipo == TRANSPORTE_PACOTES) {
            Armazem* origem = logistica.encontrarArmazem(evento.idArmazemOrigem);
            Pacote* pacote = origem->retirarPacote(evento.idArmazemDestino);
            
            if (pacote) {
                evento.pacote = pacote;
                evento.imprimirEvento("removido");
                evento.imprimirEvento("transito");
                
                pacote->avancarRota();
                
                if (pacote->rota.is_empty() && evento.idArmazemDestino == pacote->idArmazemDestino) {
                    Evento eventoEntrega(evento.tempo + 20, TRANSPORTE_PACOTES, 
                                       evento.idArmazemDestino, evento.idArmazemDestino);
                    eventoEntrega.pacote = pacote;
                    eventoEntrega.imprimirEvento("entregue");
                    delete pacote;
                }
                else {
                    pacote->idArmazemOrigem = evento.idArmazemDestino;
                    Evento eventoChegada(evento.tempo + 20, CHEGADA_PACOTE, pacote);
                    eventoChegada.idArmazemOrigem = evento.idArmazemDestino;
                    eventoChegada.idArmazemDestino = pacote->getProximoDestino();
                    escalonador.insereEvento(eventoChegada);
                }
            }
        }
    }

    return 0;
}