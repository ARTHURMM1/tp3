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
    int id;                 // ID original do arquivo, para lógica e desempate
    int idImpressao;        // ID sequencial (0, 1, ...), para impressão
    int idArmazemOrigem;
    int idArmazemDestino;
    EstadoPacote estado;
    List<int> rota; 
    
    double tempoPostagem;
    double tempoTotalArmazenado;
    double tempoEntradaArmazem; 

    // Construtor atualizado para receber ambos os IDs
    Pacote(int id_original = -1, int id_impr = -1, int origem = -1, int destino = -1, double tempo = 0.0)
        : id(id_original),
          idImpressao(id_impr),
          idArmazemOrigem(origem),
          idArmazemDestino(destino),
          estado(NAO_POSTADO),
          tempoPostagem(tempo),
          tempoTotalArmazenado(0.0),
          tempoEntradaArmazem(0.0)
          {}
    
    int getProximoDestino() {
        if (!rota.is_empty()) {
            return rota.front();
        }
        return -1; 
    }
   
    void avancarRota() {
        if (!rota.is_empty()) {
            rota.pop_front();
        }
    }

    // PROÍBE CÓPIA
    Pacote(const Pacote&) = delete;
    Pacote& operator=(const Pacote&) = delete;
};

#endif