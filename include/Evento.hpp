#ifndef EVENTO_HPP
#define EVENTO_HPP

#include "Pacote.hpp"
#include <iostream>
#include <iomanip>
#include <string>

#define EVENTO_PACOTE 1
#define EVENTO_TRANSPORTE 2

inline void printPaddedNumber(int value, int width) {
    std::cout << std::setw(width) << std::setfill('0') << value;
}

class Evento {
public:
    double tempo;
    int tipo;
    Pacote* pacote;
    int idArmazemOrigem;
    int idArmazemDestino;
    long long chave;

    Evento(double t, int tip, Pacote* pac)
        : tempo(t), tipo(tip), pacote(pac), idArmazemOrigem(-1), idArmazemDestino(-1) {
        // A chave continua usando o ID original para desempate
        this->chave = (long long)tempo * 10000000LL + (long long)(pacote ? pacote->id : -1) * 10LL + EVENTO_PACOTE;
        if (pacote) {
            this->idArmazemOrigem = pacote->idArmazemOrigem;
        }
    }
    
    Evento(double t, int tip, int orig, int dest)
        : tempo(t), tipo(tip), pacote(nullptr), idArmazemOrigem(orig), idArmazemDestino(dest) {
        this->chave = (long long)tempo * 10000000LL + (long long)orig * 10000LL + (long long)dest * 10LL + EVENTO_TRANSPORTE;
    }

    Evento() : tempo(0), tipo(0), pacote(nullptr), idArmazemOrigem(-1), idArmazemDestino(-1), chave(0) {}

    bool operator<(const Evento& outro) const {
        return this->chave < outro.chave;
    }

    bool operator>(const Evento& outro) const {
        return this->chave > outro.chave;
    }

    void imprimir() const {
        printPaddedNumber((int)tempo, 7);
        std::cout << " pacote ";
        // --- CORREÇÃO: Usa o idImpressao para o log ---
        printPaddedNumber(pacote ? pacote->idImpressao : -1, 3);

        if (tipo == EVENTO_PACOTE) {
            if (pacote && pacote->estado == ENTREGUE) {
                std::cout << " entregue em ";
                printPaddedNumber(idArmazemOrigem, 3);
            } else if (pacote && pacote->estado == ARMAZENADO) {
                 std::cout << " armazenado em ";
                 printPaddedNumber(idArmazemOrigem, 3);
                 std::cout << " na secao ";
                 printPaddedNumber(idArmazemDestino, 3);
            }
        }
        std::cout << std::endl;
    }

    void imprimir(const std::string& msg) const {
        printPaddedNumber((int)tempo, 7);
        std::cout << " pacote ";
        // --- CORREÇÃO: Usa o idImpressao para o log ---
        printPaddedNumber(pacote ? pacote->idImpressao : -1, 3);
        std::cout << " " << msg << " ";
        printPaddedNumber(idArmazemOrigem, 3);

        if (msg == "em transito de") {
            std::cout << " para ";
            printPaddedNumber(idArmazemDestino, 3);
        } else {
            std::cout << " na secao ";
            printPaddedNumber(idArmazemDestino, 3);
        }
        std::cout << std::endl;
    }
};

#endif