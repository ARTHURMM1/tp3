#ifndef EVENTO_HPP
#define EVENTO_HPP

#include "Pacote.hpp"
#include <iostream>
#include <iomanip>

inline void printPaddedNumber(int value, int width) {
    std::string s = std::to_string(value);
    int padding = width - s.length();
    for (int i = 0; i < padding; ++i) std::cout << "0";
    std::cout << s;
}

class Evento {
public:
    double tempo;
    int tipo;
    Pacote* pacote;
    int idArmazemOrigem;
    int idArmazemDestino;

    Evento(double t, int tip, Pacote* pac)
        : tempo(t), tipo(tip), pacote(pac), idArmazemOrigem(-1), idArmazemDestino(-1) {}

    Evento(double t, int tip, int orig, int dest)
        : tempo(t), tipo(tip), pacote(nullptr), idArmazemOrigem(orig), idArmazemDestino(dest) {}

    Evento() : tempo(0), tipo(0), pacote(nullptr), idArmazemOrigem(-1), idArmazemDestino(-1) {}

    void imprimirEvento(const std::string& tipoMsg, int origem, int destino = -1, int secao = -1) const {
        printPaddedNumber((int)tempo, 7);
        std::cout << " pacote ";
        printPaddedNumber(pacote ? pacote->id : -1, 3);

        if (tipoMsg == "armazenado") {
            std::cout << " armazenado em ";
            printPaddedNumber(origem, 3);
            std::cout << " na secao ";
            printPaddedNumber(secao + 1, 3);
        } 
        else if (tipoMsg == "removido") {
            std::cout << " removido de ";
            printPaddedNumber(origem, 3);
            std::cout << " na secao ";
            printPaddedNumber(secao + 1, 3);
        }
        else if (tipoMsg == "transito") {
            std::cout << " em transito de ";
            printPaddedNumber(origem, 3);
            std::cout << " para ";
            printPaddedNumber(destino, 3);
        }
        else if (tipoMsg == "entregue") {
            std::cout << " entregue em ";
            printPaddedNumber(origem, 3);
        }
        std::cout << std::endl;
    }
};

#define CHEGADA_PACOTE 0
#define TRANSPORTE_PACOTES 1

#endif