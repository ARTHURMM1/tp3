#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>
#include "List.hpp"

// Estados possíveis de um pacote, conforme especificação 
enum EstadoPacote {
    NAO_POSTADO,
    CHEGADA_ESCALONADA,
    AGUARDANDO_ARMAZENAMENTO,
    ARMAZENADO,
    ALOCADO_TRANSPORTE,
    ENTREGUE
};


class Pacote {
public:
    int id;
    int idArmazemOrigem;
    int idArmazemDestino;
    EstadoPacote estado;
    List<int> rota; // Rota como uma lista de IDs de armazéns
    
    // Para estatísticas
    double tempoPostagem;
    double tempoTotalArmazenado;
    double tempoEntradaArmazem; // Para calcular tempo de estadia

    Pacote(int id = -1, int origem = -1, int destino = -1, double tempo = 0.0)
        : id(id),
          idArmazemOrigem(origem),
          idArmazemDestino(destino),
          estado(NAO_POSTADO),
          tempoPostagem(tempo),
          tempoTotalArmazenado(0.0),
          tempoEntradaArmazem(0.0)
          {}
    
    // Retorna o próximo armazém na rota
    int getProximoDestino() {
        if (!rota.is_empty()) {
            return rota.front();
        }
        return -1; // Sem próximo destino
    }

    // Avança para o próximo passo na rota
    void avancarRota() {
        if (!rota.is_empty()) {
            rota.pop_front();
        }
    }
};

#endif