#include "split.h"

//Splitting algorithm
unsigned split(std::vector<Hamiltonian*> stack ,unsigned number_of_threads, unsigned backup_threshold, std::string file_name)
{
    std::vector<Hamiltonian*> tstack;
    std::vector<Hamiltonian*>* request_stack;
    Hamiltonian* temp_pointer = NULL;
    unsigned number_of_hamiltonians = 0,thread_counter = 0;
    unsigned tn,active_threads,request_node, backup_thread, temp_counter, backup_counter=0;
    TelephoneCenter gmail;

    gmail.file_name = get_filename(file_name);

    omp_set_dynamic(0);
    omp_set_num_threads(number_of_threads+1);
    #pragma omp parallel shared(number_of_hamiltonians,gmail) firstprivate(tn,tstack,temp_pointer,request_node,request_stack,thread_counter,backup_counter,temp_counter)
    {
        tn = omp_get_thread_num();
        if(tn == 0)
        {
            active_threads = omp_get_num_threads()-1;

            std::cout << "Initializing " << active_threads << " worker thread(s) and 1 backup thread" << std::endl;

            gmail.resize(active_threads+1);
            gmail.is_working[tn] = true;

            tstack = stack;
            backup_thread = active_threads;

            std::cout << "Initialization complete" << std::endl;
            std::cout << "Executing..." << std::endl;
        }

        #pragma omp barrier
        if(tn != backup_thread)
            gmail.telephone_book[tn] = &tstack;
        #pragma omp barrier

        while(!gmail.all_states_idle())
        {
            //Periodic backup routine
            if(gmail.is_working[backup_thread])
            {
                #pragma omp barrier
                if(tn == active_threads)
                {
                    #pragma omp critical
                    gmail.backup();
                    std::cout << "Backup complete" << std::endl;
                    gmail.is_working[backup_thread]= false;
                }
                #pragma omp barrier
            }
            else if(tn == backup_thread)
            {
                usleep(1000000);
                temp_counter = gmail.current_number_of_hamiltonians();

                if(temp_counter-backup_counter >= backup_threshold)
                {
                    backup_counter = temp_counter;
                    if(!gmail.all_states_idle())
                    {
                        std::cout << "Backing up" << std::endl;
                        gmail.is_working[backup_thread]= true;
                    }
                }
            }
            else
            {
                //Actual splitting algorithm
                if(tstack.size()==0)
                {
                    usleep(1000000);
                    gmail.is_working[tn] = false;
                    gmail.send_email(tn,&tstack);
                }
                else if(tstack.back()->is_simple())
                {
                    gmail.is_working[tn] = true;
                    thread_counter++;
                    gmail.counters[tn]++;
                    delete tstack.back();
                    tstack.pop_back();
                    gmail.telephone_book[tn] = &tstack;
                }
                else
                {
                    gmail.is_working[tn] = true;
                    temp_pointer = tstack.back();
                    tstack.pop_back();
                    tstack.push_back(temp_pointer->split_right());
                    tstack.push_back(temp_pointer->split_left());
                    delete temp_pointer;
                    temp_pointer = NULL;

                    request_node=gmail.check_first_email_sender(tn);
                    if(request_node!=gmail.end())
                    {
                        if(tstack.size()>1)
                        {
                            gmail.transfer(tn,request_node,&tstack);
                        }
                    }

                    gmail.telephone_book[tn] = &tstack;
                }
            }
        }
        #pragma omp critical
        number_of_hamiltonians+=thread_counter;
    }
    return number_of_hamiltonians;
}

