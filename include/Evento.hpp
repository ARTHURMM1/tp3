#ifndef EVENTO_HPP
#define EVENTO_HPP

#include "Pacote.hpp"


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

        Evento() {}
    
        Evento(double t, TipoEvento tp, Pacote* p)
            : tempo(t), tipo(tp), pacote(p), idArmazemOrigem(-1), idArmazemDestino(-1) {}

        Evento(double t, TipoEvento tp, int origem, int destino)
            : tempo(t), tipo(tp), pacote(nullptr), idArmazemOrigem(origem), idArmazemDestino(destino) {}

        bool operator>(const Evento& other) const {
            return this->tempo > other.tempo;
        }
};

#endif