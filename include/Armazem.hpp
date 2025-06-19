#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Stack.hpp"
#include "Pacote.hpp"

#define MAX_SIZE 100

class Armazem {
    private:
    int id;
    Stack<Pacote*> mainstack;
   

    public:
    Armazem (int id) : id(id) {}
    ~Armazem() {}

    int get_id() const {return id;}
};


#endif