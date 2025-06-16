#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Stack.hpp"
#include "Pacote.hpp"

#define MAX_SIZE 100

class Armazem {
    private:
    int id;
    Stack<Pacote> mainstack;
    Stack<Pacote>* substacks;

    public:
    Armazem (int id) : mainstack(MAX_SIZE), substacks(new Stack<Pacote>[MAX_SIZE]) {}
    ~Armazem() {delete[] substacks;}

    int get_id() const {return id;}
};


#endif