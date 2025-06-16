#ifndef VERTICEARMAZEM_HPP
#define VERTICEARMAZEM_HPP
#include "Armazem.hpp"
#include"List.hpp"

class VerticeArmazem {
    
    public: 
    VerticeArmazem(const Armazem& a) : storage(a) {}
    ~VerticeArmazem() {}

    Armazem storage;
    List<VerticeArmazem*> vizinhos;

};


#endif