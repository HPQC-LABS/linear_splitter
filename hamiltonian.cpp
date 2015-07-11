#include "hamiltonian.h"

using std::to_string;

Hamiltonian::Hamiltonian(int m, int n, int N):n_(n),m_(m),N_(N)
{
    read_hamiltonian(m,n,N);
    sort_terms();
    //Initialize variable costs
    for(unsigned i = 0; i < counter_; i++)
        variables_[i] = 0;
    //Calculate highest cost variable
    split_variable = split_by_total_cost();
}

Hamiltonian::Hamiltonian(std::vector<edge_type> edges,std::map<unsigned,unsigned> variables)
{
    edges_ = edges;
    sort_terms();
    variables_=variables;
    for(map_iterator it = variables_.begin(); it!=variables_.end(); ++it)
        it -> second = 0;
    split_variable = split_by_total_cost();
}

void Hamiltonian::sort_terms()
{
    for(unsigned i = 0; i < edges_.size(); ++i)
        std::sort(edges_[i].first.begin(),edges_[i].first.end());
}

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

    return new Hamiltonian(temp_edges,temp_variables);
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
                temp_edges.erase(temp_edges.begin()+i);
                if(temp_edges[j].second == 0)
                {
                    temp_edges.erase(temp_edges.begin()+j);
                    --i;
                }
                --i;
            }
        }
    }
    temp_variables.erase(split_variable);
    return new Hamiltonian(temp_edges,temp_variables);
}

bool Hamiltonian::is_simple()
{
    if(cost() < qubits)
        return true;
    return false;
}

void Hamiltonian::read_hamiltonian(int m, int n, int N)
{
    //Determine file name
    std::string const file_name = "H_" + to_string(m) + "_" + to_string(n) + "_" + to_string(N) + ".txt";

    std::ifstream in(file_name);
    assert(in);

    counter_ = 0;
    std::map<std::string,unsigned> index;
    while(in){

        std::string input_str; 
        if(!std::getline(in,input_str)) break;

        if(input_str.find('#') == std::string::npos){

            //Read line into vector input
            std::istringstream tmp0(input_str);
            std::vector<std::string> input;
            while (tmp0){
                std::string s;
                if (!std::getline(tmp0, s, ' ')) break;
                input.push_back(s);
            }

            const int val(std::stoi(input[input.size()-1]));

            edge_type edge;

            edge.second = val;

            for(unsigned i = 0; i < input.size()-1; ++i){
                const std::string site(input[i]);
                //If site does not appear in the map index
                if(index.find(site) == index.end())
                    index[site] = counter_++;
                edge.first.push_back(index[site]);
            }
            edges_.push_back(edge);
        }
    }
    in.close();
}

unsigned Hamiltonian::split_by_total_terms(){
    for(unsigned j = 0; j < edges_.size(); ++j)
        for(const auto a : edges_[j].first)
            variables_[a]++;
    return max_map_value(variables_.begin(), variables_.end()) -> first;
}

unsigned Hamiltonian::split_by_total_cost()
{
    for(unsigned j = 0; j < edges_.size(); ++j)
        for(const auto a : edges_[j].first)
            variables_[a] += edges_[j].first.size() - 2 + 1;
    return max_map_value(variables_.begin(), variables_.end()) -> first; 
}

unsigned Hamiltonian::cost()
{
    unsigned h_cost = variables_.size();
    for(unsigned j = 0; j < edges_.size(); ++j)
        if(edges_[j].first.size()>2)
            h_cost += edges_[j].first.size()-2;
    return h_cost; 
}


unsigned split(Hamiltonian* h)
{
    std::vector<Hamiltonian*> stack;
    Hamiltonian* current_node = h;
    Hamiltonian* temp_pointer = NULL;
    unsigned number_of_hamiltonians = 1;

    while((!current_node->is_simple())||(stack.size()>0))
    {
        if(current_node->is_simple())
        {
            ++number_of_hamiltonians;
            delete current_node;
            current_node = stack.back();
            stack.pop_back();
        }
        else
        {
            temp_pointer = current_node->split_left();
            stack.push_back(current_node->split_right());
            delete current_node;
            current_node = temp_pointer;
        }
    }
    return number_of_hamiltonians;
}
