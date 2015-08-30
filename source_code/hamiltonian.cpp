#include "hamiltonian.h"
using std::to_string;

//Initializers
Hamiltonian::Hamiltonian(unsigned number_of_qubits) 
{
    qubits=number_of_qubits;
    counter_=0;
}

Hamiltonian::Hamiltonian(std::vector<edge_type> edges,std::map<unsigned,unsigned> variables, unsigned number_of_qubits)
{
    qubits = number_of_qubits;
    edges_ = edges;
    for(unsigned i = 0; i < edges_.size(); ++i)
        std::sort(edges_[i].first.begin(),edges_[i].first.end());

    variables_ = variables;

    for(map_iterator it = variables_.begin(); it!=variables_.end(); ++it)
        it -> second = 0;

    split_variable = split_by_total_cost();
}

void Hamiltonian::clean_hamiltonian()
{
    //Sort edge list
    for(unsigned i = 0; i < edges_.size(); ++i)
        std::sort(edges_[i].first.begin(),edges_[i].first.end());
    
    //Initialize variable costs
    for(unsigned i = 0; i < counter_; i++)
        variables_[i] = 0;
    
    //Calculate highest cost variable
    split_variable = split_by_total_cost();
}

//Split functions
Hamiltonian* Hamiltonian::split_left()
{
    std::vector<edge_type> temp_edges = edges_;
    std::map<unsigned,unsigned> temp_variables = variables_;

    for(unsigned i = 0; i < temp_edges.size() ; i++)
        if(std::find(temp_edges[i].first.begin(), temp_edges[i].first.end(), split_variable)!=temp_edges[i].first.end())
        {
            temp_edges.erase(temp_edges.begin()+i);
            --i;
        }
    temp_variables.erase(split_variable);

    return new Hamiltonian(temp_edges,temp_variables, qubits);
}

Hamiltonian* Hamiltonian::split_right()
{
    std::vector<edge_type> temp_edges = edges_;
    std::map<unsigned,unsigned> temp_variables = variables_;
    std::vector<unsigned>::iterator position;
    unsigned j;

    for(unsigned i = 0; i < temp_edges.size() ; ++i)
    {
        position = std::find(temp_edges[i].first.begin(), temp_edges[i].first.end(), split_variable);
        if(position != temp_edges[i].first.end())
        {
            temp_edges[i].first.erase(position);
            j = 0;
            for(; j < i; ++j)
                if(temp_edges[i].first==temp_edges[j].first)
                    break;
            if(j!=i)
            {
                temp_edges[j].second += temp_edges[i].second;
                temp_edges.erase(temp_edges.begin()+i); if(temp_edges[j].second == 0)
                {
                    temp_edges.erase(temp_edges.begin()+j);
                    --i;
                }
                --i;
            }
        }
    }
    temp_variables.erase(split_variable);
    return new Hamiltonian(temp_edges,temp_variables,qubits);
}

//C(H) functions
bool Hamiltonian::is_simple()
{
    if(cost() <= qubits)
        return true;
    return false;
}

unsigned Hamiltonian::cost()
{
    unsigned h_cost = 0;
    for(unsigned j = 0; j < variables_.size(); ++j)
        if(variables_[j]>0)
            ++h_cost;
    for(unsigned j = 0; j < edges_.size(); ++j)
        if(edges_[j].first.size()>2)
            h_cost += edges_[j].first.size()-2;
    return h_cost; 
}

//C_H functions
unsigned Hamiltonian::split_by_total_terms(){
    for(unsigned j = 0; j < edges_.size(); ++j)
        for(std::vector<unsigned>::iterator a = edges_[j].first.begin(); a != edges_[j].first.end(); ++a)
            variables_[*a]++;
    return max_map_value(variables_.begin(), variables_.end()) -> first;
}

unsigned Hamiltonian::split_by_total_cost()
{
    for(unsigned j = 0; j < edges_.size(); ++j)
    {
        for(std::vector<unsigned>::iterator a = edges_[j].first.begin(); a != edges_[j].first.end(); ++a)
        {
            if(variables_[*a]==0)
                ++variables_[*a];
            if(edges_[j].first.size()>=2)
                variables_[*a] += edges_[j].first.size() - 2 + 1;
        }
    }
    return max_map_value(variables_.begin(), variables_.end()) -> first; 
}
