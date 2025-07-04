#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <iostream>
#include <iomanip>

struct Evento {
    int dataHora;
    std::string tipoConsulta; // EV, PC, CL
    std::string linhaOriginal; // Armazena a linha completa para impressão

    // Para eventos EV
    int idPacote;
    std::string tipoEvento; // RG, AR, RM, UR, TR, EN
    
    // Construtor para consultas
    Evento(int dh, const std::string& tipo, const std::string& linha)
        : dataHora(dh), tipoConsulta(tipo), linhaOriginal(linha), idPacote(-1) {}

    // Construtor para eventos
    Evento(int dh, int id, const std::string& tipoEv, const std::string& linha)
        : dataHora(dh), tipoConsulta("EV"), linhaOriginal(linha), idPacote(id), tipoEvento(tipoEv) {}

    // Imprime o evento no formato original
    void imprimir() const {
        std::cout << linhaOriginal << std::endl;
    }
};

#endif