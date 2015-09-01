#include "hamiltonian.h"

//Splitting algorithm
unsigned split(std::vector<Hamiltonian*> stack ,unsigned number_of_threads)
{
    std::vector<Hamiltonian*> tstack;
    std::vector<Hamiltonian*>* request_stack;
    Hamiltonian* temp_pointer = NULL;
    unsigned number_of_hamiltonians = 0,thread_counter = 0;
    unsigned tn,nthreads,request_node;
    TelephoneCenter gmail;

    omp_set_dynamic(0);
    omp_set_num_threads(number_of_threads);
    #pragma omp parallel shared(number_of_hamiltonians,gmail) firstprivate(tn,tstack,temp_pointer,request_node,request_stack,thread_counter)
    {
        tn = omp_get_thread_num();
        #pragma omp single 
        {
            nthreads = omp_get_num_threads();
            std::cout << "Initializing " << nthreads << " threads..." << std::endl;
            gmail.resize(nthreads);
            tstack = stack;
            gmail.is_working[tn] = 1;
            std::cout << "Executing..." << std::endl;
        }

        while(!gmail.all_states_idle())
        {
            if(tstack.size()==0)
            {
                if(gmail.has_reply(tn))
                {
                    gmail.accept_transfer(tn,&tstack);
                    #pragma omp flush (gmail)
                    gmail.is_working[tn] = 1;
                }
                else
                {
                    usleep(1000000);
                    if(gmail.is_working[tn]!=-1)
                        gmail.is_working[tn] = 0;
                    gmail.send_email(tn,&tstack);
                }
            }
            else if(tstack.back()->is_simple())
            {
                //replace with local counter
                gmail.is_working[tn] = 1;
                ++thread_counter;
                delete tstack.back();
                tstack.pop_back();
            }
            else
            {
                gmail.is_working[tn] = 1;
                temp_pointer = tstack.back();
                tstack.pop_back();
                tstack.push_back(temp_pointer->split_right());
                tstack.push_back(temp_pointer->split_left());
                delete temp_pointer;
                temp_pointer = NULL;

                request_node=gmail.check_first_email_sender(tn);
                if(request_node!=gmail.end())
                {
                    if(tstack.size()>5)
                    {
                        if(gmail.transfer(tn,request_node,&tstack))
                        {
                            #pragma omp flush (gmail)
                        }
                    }
                }
            }
        }
        #pragma omp critical
        number_of_hamiltonians+=thread_counter;
    }
    return number_of_hamiltonians;
}

