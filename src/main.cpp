#include <iostream>
#include <iomanip>
#include "Log.hpp"
#include "Escalonador.hpp"
#include "Pacote.hpp"

// Função auxiliar para encontrar o índice da seção (destino) em um armazém
int encontrarIndiceSecao(Armazem* armazem, int idDestino) {
    int idx = 0;
    List<PilhaPorDestino>::Node* curr = armazem->secoes.get_head();
    while (curr) {
        if (curr->data.idDestino == idDestino) return idx;
        curr = curr->next;
        idx++;
    }
    return -1; // Não encontrado
}

int main() {
    // Leitura dos parâmetros do sistema logístico
    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao, numeroArmazens;
    std::cin >> capacidadeTransporte >> latenciaTransporte >> intervaloTransportes >> custoRemocao >> numeroArmazens;

    Log logistica;
    Escalonador escalonador;

    // Criação dos armazéns
    for (int i = 0; i < numeroArmazens; i++) {
        Armazem* armazem = new Armazem(i);
        logistica.add_storage(armazem);
    }

    // Leitura da matriz de adjacência e criação das rotas
    for (int i = 0; i < numeroArmazens; i++) {
        for (int j = 0; j < numeroArmazens; j++) {
            int conexao;
            std::cin >> conexao;
            if (conexao == 1) {
                logistica.add_path(i, j);
            }
        }
    }

    // Leitura do número de pacotes
    int numeroPacotes;
    std::cin >> numeroPacotes;

    // Leitura dos pacotes e agendamento dos eventos de chegada
    for (int i = 0; i < numeroPacotes; i++) {
        int tempoChegada, idPacote, origem, destino;
        std::string dummy;
        std::cin >> tempoChegada >> dummy >> idPacote >> dummy >> origem >> dummy >> destino;

        Pacote* pacote = new Pacote(idPacote, origem, destino, tempoChegada);
        pacote->estado = CHEGADA_ESCALONADA;
        pacote->rota = logistica.calcularRota(origem, destino);

        Evento eventoChegada(tempoChegada, CHEGADA_PACOTE, pacote);
        eventoChegada.idArmazemOrigem = origem;
        eventoChegada.idArmazemDestino = destino;
        escalonador.insereEvento(eventoChegada);
    }

    // Processamento dos eventos
    while (!escalonador.is_empty()) {
        Evento evento = escalonador.retiraProximoEvento();

        if (evento.tipo == CHEGADA_PACOTE) {
            Pacote* pacote = evento.pacote;
            Armazem* armazem = logistica.encontrarArmazem(pacote->idArmazemOrigem);

            int proximoDestino = pacote->getProximoDestino();
            if (proximoDestino == -1) proximoDestino = pacote->idArmazemDestino;

            armazem->armazenarPacote(pacote, proximoDestino);
            int indiceSecao = encontrarIndiceSecao(armazem, proximoDestino);

            evento.idArmazemDestino = proximoDestino;
            evento.imprimirEvento("armazenado", pacote->idArmazemOrigem, -1, indiceSecao);

            // Agenda transporte se necessário
            double tempoTransporte = evento.tempo;
            while ((int)tempoTransporte % intervaloTransportes != 0) tempoTransporte++;
            if (tempoTransporte <= 1000000) { // Ajuste conforme limite de tempo do seu simulador
                Evento eventoTransporte(tempoTransporte, TRANSPORTE_PACOTES, pacote->idArmazemOrigem, proximoDestino);
                eventoTransporte.pacote = pacote;
                escalonador.insereEvento(eventoTransporte);
            }
        }
        else if (evento.tipo == TRANSPORTE_PACOTES) {
            Armazem* origem = logistica.encontrarArmazem(evento.idArmazemOrigem);
            Pacote* pacote = origem->retirarPacote(evento.idArmazemDestino);

            if (pacote) {
                int indiceSecao = encontrarIndiceSecao(origem, evento.idArmazemDestino);
                evento.pacote = pacote;
                evento.imprimirEvento("removido", evento.idArmazemOrigem, -1, indiceSecao);
                evento.imprimirEvento("transito", evento.idArmazemOrigem, evento.idArmazemDestino);

                pacote->avancarRota();

                if (pacote->rota.is_empty() && evento.idArmazemDestino == pacote->idArmazemDestino) {
                    evento.imprimirEvento("entregue", evento.idArmazemDestino);
                    delete pacote;
                    continue; // Não agende mais nada para esse pacote!
                } else {
                    pacote->idArmazemOrigem = evento.idArmazemDestino;
                    Evento eventoChegada(evento.tempo + latenciaTransporte, CHEGADA_PACOTE, pacote);
                    eventoChegada.idArmazemOrigem = evento.idArmazemDestino;
                    eventoChegada.idArmazemDestino = pacote->getProximoDestino();
                    escalonador.insereEvento(eventoChegada);
                }
            }
        }
    }

    return 0;
}