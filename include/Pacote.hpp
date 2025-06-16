#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <iostream>
#include <string>
#include "Stats.hpp"
#include "Armazem.hpp"

class Pacote {
    private:
        int id;
        std::string from;
        std::string to;
        Armazem origin;
        Armazem destination;
        std::string type;
        Stats status;
        List<Armazem> path;

    public:
        Pacote () 
            : id(-1),
            from("none"),
            to("none"),
            origin(0),
            destination(0),
            type("none")
        {}
    
};

#endif