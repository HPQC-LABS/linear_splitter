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
#include <omp.h>

typedef std::pair<std::vector<unsigned>, int> edge_type;

class Hamiltonian{
public:
    Hamiltonian (int, int, int);
    Hamiltonian (std::vector<edge_type>,std::map<unsigned,unsigned>);
    Hamiltonian* split_left();
    Hamiltonian* split_right();
    bool is_simple();
    std::vector<edge_type> edges_;
    std::map<unsigned,unsigned> variables_;
    unsigned cost();
private:
    void read_hamiltonian (int, int, int);
    void sort_terms();

    unsigned split_by_total_terms();
    unsigned split_by_total_cost();

    int n_,m_,N_;
    const unsigned qubits = 2048;
    unsigned split_variable;
    std::size_t counter_;
};

bool all_nodes_are_simple(std::vector<bool>);
unsigned split(Hamiltonian*);

#endif
