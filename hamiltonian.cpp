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

bool all_nodes_are_simple(std::vector<bool> thread_states)
{
    for(unsigned i = 0; i < thread_states.size(); ++i)
        if(thread_states[i]==false)
            return false;
    return true;
}

unsigned split(Hamiltonian* h)
{
    std::vector<Hamiltonian*> stack;
    std::vector<Hamiltonian*> tstack;
    Hamiltonian* thread_node = NULL;
    Hamiltonian* temp_left_pointer = NULL;
    Hamiltonian* temp_right_pointer = NULL;
    unsigned number_of_hamiltonians = 0;
    unsigned tn,nthreads;
    std::vector<bool> thread_states;
    double percent = 0.75;
    unsigned allocate;

    #pragma omp parallel shared(nthreads,thread_states,stack,number_of_hamiltonians) firstprivate(tn,thread_node,tstack,temp_left_pointer,temp_right_pointer,allocate,percent)
    {
        tn = omp_get_thread_num();
        #pragma omp single 
        {
            nthreads = omp_get_num_threads();
            thread_states.resize(nthreads,true);
            thread_node = h;
            thread_states[tn]=false;
        }
        
        while(!all_nodes_are_simple(thread_states))
        {
            if(thread_node==NULL)
            {
                //Include repeated if statements to avoid having to unneccessarily enter a critical section of the code
                if(stack.size()>0)
                {
                    #pragma omp critical
                    {
                        if(stack.size()>0)
                        {
                            if(number_of_simple_nodes(thread_states)>0)
                            {
                                allocate = ceil(stack.size()/number_of_simple_nodes(thread_states));
                                std::cout << allocate << std::endl;
                                tstack.insert(tstack.end(),stack.begin(),stack.begin()+allocate-1);
                                thread_node = *(stack.begin()+allocate);
                                stack.erase(stack.begin(),stack.begin()+allocate);
                                thread_states[tn] = false;
                            }
                        }
                    }
                }
            }
            else if(thread_node->is_simple())
            {
                #pragma omp critical
                {
                    ++number_of_hamiltonians;
                    delete thread_node;
                    thread_node = NULL;
                }
                if(tstack.size()==0)
                {
                    #pragma omp critical
                    {
                        if(stack.size()>0)
                        {
                            allocate = ceil(stack.size()/(number_of_simple_nodes(thread_states)+1));
                            std::cout << allocate << std::endl;
                            tstack.insert(tstack.end(),stack.begin(),stack.begin()+allocate-1);
                            thread_node = *(stack.begin()+allocate);
                            stack.erase(stack.begin(),stack.begin()+allocate);
                        }
                        else
                        {
                            thread_states[tn]=true;
                        }
                    }
                }
                else
                {
                    thread_node = tstack.back();
                    tstack.pop_back();
                }
            }
            else
            {
                temp_left_pointer = thread_node->split_left();
                temp_right_pointer = thread_node->split_right();
                tstack.push_back(temp_right_pointer);
                if(stack.size()==0)
                {
                    if(tstack.size()>100)
                    {
                        #pragma omp critical
                        {
                            allocate = floor(tstack.size()*percent);
                            stack.insert(stack.end(),tstack.begin(),tstack.begin()+allocate);
                            tstack.erase(tstack.begin(),tstack.begin()+allocate);
                        }
                    }
                }
                delete thread_node;
                thread_node = temp_left_pointer;
            }
        }
    }
    return number_of_hamiltonians;
}
