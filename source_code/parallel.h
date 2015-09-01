//parallel.h

#ifndef PARALLEL_H_INCLUDED 
#define PARALLEL_H_INCLUDED 

#include "algorithms.h"
#include "hamiltonian.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <random>
#include <unistd.h>
#include <omp.h>

typedef std::pair<int,std::vector<Hamiltonian*> > request;

//Class allowing communication between threads
class TelephoneCenter
{
public:
    TelephoneCenter();
    TelephoneCenter(unsigned);

    std::vector<int> is_working;
    std::vector<std::vector<request> > emails;
    std::vector<std::vector<Hamiltonian*>*> telephone_book;
    std::vector<unsigned> counters;

    std::string file_name;
    std::string stack_backup_file_name;
    std::string counter_backup_file_name;

    bool all_states_idle();
    bool backup_version;
    bool transfer(int,int,std::vector<Hamiltonian*>*);
    bool has_reply(int);

    void backup();
    void resize(unsigned);
    void send_email(int,std::vector<Hamiltonian*>*);
    void accept_transfer(int, std::vector<Hamiltonian*>*);
    void delete_sent_emails(int);

    unsigned check_first_email_sender(int);
    unsigned number_of_emails_in_inbox(int);
    unsigned number_of_emails_sent(int);
    unsigned current_number_of_hamiltonians();
    unsigned count_active_states();
    unsigned end();
private:
    unsigned nthreads;
    unsigned count_idle_states();

    std::default_random_engine generator;

    bool bernoulli(unsigned);
};

#endif
