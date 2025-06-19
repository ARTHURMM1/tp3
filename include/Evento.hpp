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
    
    void imprimirEvento(const std::string& tipoMsg, int origem = -1, int destino = -1, int secao = -1) const {
        std::cout << std::setfill('0') << std::setw(7) << (int)tempo;
        std::cout << " pacote " << std::setfill('0') << std::setw(3) << (pacote ? pacote->id : -1);

        if (tipoMsg == "armazenado") {
            std::cout << " armazenado em " << std::setfill('0') << std::setw(3) 
                      << origem << " na secao " << std::setfill('0') 
                      << std::setw(3) << secao;
        } 
        else if (tipoMsg == "removido") {
            std::cout << " removido de " << std::setfill('0') << std::setw(3) 
                      << origem << " na secao " << std::setfill('0') 
                      << std::setw(3) << secao;
        }
        else if (tipoMsg == "transito") {
            std::cout << " em transito de " << std::setfill('0') << std::setw(3) 
                      << origem << " para " << std::setfill('0') 
                      << std::setw(3) << destino;
        }
        else if (tipoMsg == "entregue") {
            std::cout << " entregue em " << std::setfill('0') << std::setw(3) 
                      << origem;
        }
        std::cout << std::endl;
    }
};

// Exemplo como função global (pode ser adaptada para método da classe Evento)
void imprimirEvento(int tempo, int idPacote, const std::string& tipo, 
                    int origem, int destino = -1, int secao = -1) {
    std::cout << std::setfill('0') << std::setw(7) << tempo;
    std::cout << " pacote " << std::setfill('0') << std::setw(3) << idPacote;

    if (tipo == "armazenado") {
        std::cout << " armazenado em " << std::setfill('0') << std::setw(3) 
                  << origem << " na secao " << std::setfill('0') 
                  << std::setw(3) << secao;
    } 
    else if (tipo == "removido") {
        std::cout << " removido de " << std::setfill('0') << std::setw(3) 
                  << origem << " na secao " << std::setfill('0') 
                  << std::setw(3) << secao;
    }
    else if (tipo == "transito") {
        std::cout << " em transito de " << std::setfill('0') << std::setw(3) 
                  << origem << " para " << std::setfill('0') 
                  << std::setw(3) << destino;
    }
    else if (tipo == "entregue") {
        std::cout << " entregue em " << std::setfill('0') << std::setw(3) 
                  << origem;
    }
    std::cout << std::endl;
}

#endif