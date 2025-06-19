#ifndef EVENTO_HPP
#define EVENTO_HPP

#include "Pacote.hpp"
#include <iostream>
#include <iomanip>

enum TipoEvento {
    CHEGADA_PACOTE, 
    TRANSPORTE_PACOTES 
};

class Evento {
public:
    double tempo;
    TipoEvento tipo;
    Pacote* pacote;
    int idArmazemOrigem;
    int idArmazemDestino;

    // Construtores
    Evento() : tempo(0), tipo(CHEGADA_PACOTE), pacote(nullptr), 
               idArmazemOrigem(-1), idArmazemDestino(-1) {}

    // Para eventos de chegada de pacote
    Evento(double t, TipoEvento tip, Pacote* pac) 
        : tempo(t), tipo(tip), pacote(pac), 
          idArmazemOrigem(-1), idArmazemDestino(-1) {}

    // Para eventos de transporte
    Evento(double t, TipoEvento tip, int orig, int dest) 
        : tempo(t), tipo(tip), pacote(nullptr),
          idArmazemOrigem(orig), idArmazemDestino(dest) {}

    // Operador de comparação para ordenação no escalonador
    bool operator>(const Evento& other) const {
        return tempo > other.tempo;
    }

    // Método para impressão de eventos
    void imprimirEvento(const std::string& tipoMsg) const {
        std::cout << std::setfill('0') << std::setw(7) << (int)tempo;
        std::cout << " pacote " << std::setfill('0') << std::setw(3) << pacote->id;
        
        if (tipoMsg == "armazenado") {
            std::cout << " armazenado em " << std::setfill('0') << std::setw(3) 
                     << idArmazemOrigem << " na secao " << std::setfill('0') 
                     << std::setw(3) << idArmazemDestino;
        } 
        else if (tipoMsg == "removido") {
            std::cout << " removido de " << std::setfill('0') << std::setw(3) 
                     << idArmazemOrigem << " na secao " << std::setfill('0') 
                     << std::setw(3) << idArmazemDestino;
        }
        else if (tipoMsg == "transito") {
            std::cout << " em transito de " << std::setfill('0') << std::setw(3) 
                     << idArmazemOrigem << " para " << std::setfill('0') 
                     << std::setw(3) << idArmazemDestino;
        }
        else if (tipoMsg == "entregue") {
            std::cout << " entregue em " << std::setfill('0') << std::setw(3) 
                     << idArmazemOrigem;
        }
        std::cout << std::endl;
    }
};

#endif