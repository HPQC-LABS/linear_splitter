//hamiltonian.h

#ifndef HAMILTONIAN_H_INCLUDED 
#define HAMILTONIAN_H_INCLUDED 

#include "algorithms.h"
#include "parallel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <cassert>
#include <omp.h>
#include <math.h>
#include <unistd.h>

typedef std::pair<std::vector<unsigned>, int> edge_type;

class Hamiltonian{
public:
    Hamiltonian (std::string);
    Hamiltonian (std::string, unsigned);
    Hamiltonian (int, int, int);
    Hamiltonian (std::vector<edge_type>,std::map<unsigned,unsigned>);
    Hamiltonian* split_left();
    Hamiltonian* split_right();
    bool is_simple();
    unsigned cost();
    
    std::vector<edge_type> edges_;
    std::map<unsigned,unsigned> variables_;
private:
    void read_hamiltonian(std::string);
    void read_hamiltonian(std::string,unsigned);
    void initialize();
    void sort_terms();

    unsigned split_by_total_terms();
    unsigned split_by_total_cost();

    int n_,m_,N_;
    const unsigned qubits = 2048;
    unsigned split_variable;
    std::size_t counter_;
};

std::vector<Hamiltonian*> initialize_multiple(std::string);

bool all_nodes_are_simple(std::vector<bool>);
unsigned split(std::vector<Hamiltonian*>);

#endif