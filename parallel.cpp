#include "parallel.h"

TelephoneCenter::TelephoneCenter(){}

TelephoneCenter::TelephoneCenter(unsigned number_of_threads)
{
    resize(number_of_threads);
}

void TelephoneCenter::resize(unsigned number_of_threads)
{
    nthreads = number_of_threads;
    is_working.resize(nthreads);

    //The column of `emails` is the thread that is requesting work
    //and the row is the thread giving the work
    emails.resize(nthreads);
    for(unsigned i = 0; i < nthreads; ++i)
    {
        emails[i].resize(nthreads);
        for(unsigned j = 0; j < nthreads; ++j)
        {
            emails[i][j].first = false;
            emails[i][j].second = NULL;
        }

    }
}

void TelephoneCenter::delete_sent_emails(int thread_number)
{
    for(unsigned i = 0; i < nthreads; ++i)
    {
        emails[i][thread_number].first = false;
        emails[i][thread_number].second = NULL;
    }
}

void TelephoneCenter::send_email(int thread_number, std::vector<Hamiltonian*>* vector_location)
{
    if(!all_states_idle())
    {
        if(number_of_emails_sent(thread_number)==0)
        {
            unsigned receiver,counter=0;
            
            //See resevoir sampling for explanation
            for(unsigned i = 0; i < nthreads; ++i)
            {
                if(is_working[i])
                {
                    ++counter;
                    if(bernoulli(counter))
                        receiver=i;
                }
            }
            emails[receiver][thread_number] = std::make_pair(true,vector_location);
        }
    }
}

void TelephoneCenter::transfer(int thread_number, int destination, std::vector<Hamiltonian*>* local_stack)
{
    std::vector<Hamiltonian*>* request_stack = emails[thread_number][destination].second;
    unsigned transfer_amount = floor(local_stack->size()/(1+number_of_emails_in_inbox(thread_number)));
    if(transfer_amount!=0)
    {
        if(request_stack->size()==0)
        {
            std::cout << "############INITIATING TRANSFER################" << std::endl;
            std::cout << "Emails: " << number_of_emails_in_inbox(thread_number) << std::endl;
            std::cout << "Amount: " << transfer_amount << std::endl;
            std::cout << thread_number << " - sender size before: " << local_stack->size() << std::endl;
            std::cout << destination << " - receiver size before: " << request_stack->size() << std::endl;
            request_stack->insert(request_stack->end(),local_stack->begin(),local_stack->begin()+transfer_amount);
            local_stack->erase(local_stack->begin(),local_stack->begin()+transfer_amount);
            std::cout << thread_number << " - sender size after: " << local_stack->size() << std::endl;
            std::cout << destination << " - receiver size after: " << request_stack->size() << std::endl;
            std::cout << "############TERMINATING TRANSFER################" << std::endl << std::endl;

            emails[thread_number][destination].first = false; 
            emails[thread_number][destination].second = NULL; 
        }
    }
}

unsigned TelephoneCenter::count_idle_states()
{
    unsigned counter = 0;
    for(unsigned i = 0; i < nthreads; ++i)
        if(!is_working[i])
            ++counter;
    return counter;
}

unsigned TelephoneCenter::number_of_emails_in_inbox(int thread_number)
{
    unsigned counter = 0;
    for(unsigned i = 0; i < nthreads; ++i)
        if(emails[thread_number][i].first)
            ++counter;
    return counter;
}

unsigned TelephoneCenter::number_of_emails_sent(int thread_number)
{
    unsigned counter = 0;
    for(unsigned i = 0; i < nthreads; ++i)
        if(emails[i][thread_number].first)
            ++counter;
    return counter;
}

unsigned TelephoneCenter::check_first_email_sender(int thread_number)
{
    for(unsigned i = 0; i < nthreads; ++i)
        if(emails[thread_number][i].first)
            return i;
    return nthreads;
}

unsigned TelephoneCenter::end()
{
    return nthreads;
}

bool TelephoneCenter::bernoulli(unsigned n)
{
    std::bernoulli_distribution distribution(1/n);
    return distribution(generator);
}

bool TelephoneCenter::all_states_idle()
{
    for(unsigned i = 0; i < nthreads; ++i)
        if(is_working[i])
            return false;
    return true;
}
