//hamiltonian.h

#ifndef HAMILTONIAN_H_INCLUDED 
#define HAMILTONIAN_H_INCLUDED 

#include "algorithms.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <cassert>
#include <math.h>
#include <unistd.h>

typedef std::pair<std::vector<unsigned>, int> edge_type;

//hamiltonian.cpp
class Hamiltonian{
public:
    //Initializers
    Hamiltonian (unsigned, unsigned);
    Hamiltonian (std::vector<edge_type>,std::map<unsigned,unsigned>, unsigned, unsigned);

    void clean_hamiltonian(); 

    //Public methods
    Hamiltonian* split_left();
    Hamiltonian* split_right();
    bool is_simple();
    unsigned cost();
    unsigned ishikawa_cost();

    //Number of qubits available
    unsigned qubits;
    unsigned max_interaction;
    
    //Highest cost variable
    unsigned split_variable;

    std::size_t counter_;
    std::vector<edge_type> edges_;
    std::map<unsigned,unsigned> variables_;
private:

    //C_H methods
    unsigned split_by_total_terms();
    unsigned split_by_total_cost();
    unsigned split_by_ishikawa_cost();
};

//read.cpp
std::vector<Hamiltonian*> read_file(std::string, unsigned, unsigned);

#endif
