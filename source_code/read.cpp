#include "hamiltonian.h"

std::vector<Hamiltonian*> read_file(std::string input_file, unsigned qubits, unsigned max_interaction)
{
    std::ifstream in(input_file);
    assert(in);

    Hamiltonian* h = new Hamiltonian(qubits, max_interaction);
    std::vector<Hamiltonian*> initial_stack;
    std::map<std::string,unsigned> index;

    while(in){

        std::string input_str; 
        if(!std::getline(in,input_str)) break;

        if(input_str.find('#') == std::string::npos)
        {

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
                        index[site] = h->counter_++;
                    edge.first.push_back(index[site]);
                }
                h->edges_.push_back(edge);
        }
        else
        {
            index.clear();
            h->clean_hamiltonian();
            initial_stack.push_back(h);
            h = new Hamiltonian(qubits, max_interaction);
        }
    }
    in.close();
    return initial_stack;
}


