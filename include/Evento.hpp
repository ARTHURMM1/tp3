// Em um novo arquivo, por exemplo, include/Evento.hpp

#ifndef EVENTO_HPP
#define EVENTO_HPP

#include "Pacote.hpp"

// Tipos de eventos possíveis na simulação
enum TipoEvento {
    CHEGADA_PACOTE, // Pacote chega a um armazém
    TRANSPORTE_PACOTES // Transporte diário entre dois armazéns
};

class Evento {
public:
    double tempo;       // O tempo em que o evento ocorre (chave da prioridade)
    TipoEvento tipo;    // O tipo do evento
    Pacote* pacote;     // Ponteiro para o pacote associado (pode ser nullptr)
    int idArmazemOrigem; // IDs para eventos de transporte
    int idArmazemDestino;

    // Construtor para eventos de chegada
    Evento(double t, TipoEvento tp, Pacote* p)
        : tempo(t), tipo(tp), pacote(p), idArmazemOrigem(-1), idArmazemDestino(-1) {}

    // Construtor para eventos de transporte
    Evento(double t, TipoEvento tp, int origem, int destino)
        : tempo(t), tipo(tp), pacote(nullptr), idArmazemOrigem(origem), idArmazemDestino(destino) {}

    // Operador para o Min-Heap comparar eventos pelo tempo
    bool operator>(const Evento& other) const {
        return this->tempo > other.tempo;
    }
};

#endif