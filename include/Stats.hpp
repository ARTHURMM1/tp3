#ifndef STATS_HPP
#define STATS_HPP 

#include "List.hpp"

class Stats {
    private:
    bool posted;
    bool en_route;
    bool final_destination;
    bool in_mainstack;
    bool in_substack;
    
    List<Armazem> schedule;

    public:
    void set_schedule(List<Armazem> route) {
        schedule = route;
    }
};
#endif