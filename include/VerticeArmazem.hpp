#ifndef VERTICEARMAZEM_HPP
#define VERTICEARMAZEM_HPP
#include "Armazem.hpp"
#include "List.hpp"

class VerticeArmazem {
public: 
    VerticeArmazem(Armazem* a) : storage(a) {}
    ~VerticeArmazem() { delete storage; }

    Armazem* storage;
    List<VerticeArmazem*> vizinhos;
};

#endif