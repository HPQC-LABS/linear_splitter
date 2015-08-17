//parallel.h

#ifndef PARALLEL_H_INCLUDED 
#define PARALLEL_H_INCLUDED 

#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>
#include <random>
#include <omp.h>

class Hamiltonian;

typedef std::pair<bool,std::vector<Hamiltonian*>*> request;

//Class allowing communication between threads
class TelephoneCenter
{
public:
    TelephoneCenter();
    TelephoneCenter(unsigned);

    std::vector<bool> is_working;
    std::vector<std::vector<request> > emails;

    bool all_states_idle();

    void resize(unsigned);
    void send_email(int,std::vector<Hamiltonian*>*);
    void transfer(int,int,std::vector<Hamiltonian*>*);
    void delete_sent_emails(int);

    unsigned check_first_email_sender(int);
    unsigned number_of_emails_in_inbox(int);
    unsigned number_of_emails_sent(int);
    unsigned end();
private:
    unsigned nthreads;
    unsigned count_idle_states();

    std::default_random_engine generator;

    bool bernoulli(unsigned);
};

#endif
