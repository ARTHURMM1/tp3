#ifndef LOG_HPP
#define LOG_HPP

#include "List.hpp"
#include "Armazem.hpp"
#include "VerticeArmazem.hpp"

class Log {
    private:
        List<VerticeArmazem*> armazens;

    public:
        Log () {}
        ~Log () {}

        void add_storage(const Armazem& novo) {
            VerticeArmazem* vertice = new VerticeArmazem(novo);
            armazens.push_back(vertice);
        }

        void add_path(VerticeArmazem* origem, VerticeArmazem* destino) {
            origem->vizinhos.push_back(destino);
            destino->vizinhos.push_back(origem);
        }

};

#endif