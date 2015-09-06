#include "hamiltonian.h"
#include "split.h"
#include <ctime>
#include <stdlib.h>

//Compile with
//g++ split_reduc.cpp algorithms.cpp hamiltonian.cpp parallel.cpp read.cpp split.cpp -o split_reduc -std=c++11 -fopenmp -O3

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);

    unsigned number_of_threads = 8, number_of_qubits = 2048, backup_threshold = 100000, transfer_threshold = 5;

    std::vector<std::string>::iterator qubit_flag = std::find(args.begin(),args.end(),"-q");
    if(qubit_flag!=args.end())
        if(qubit_flag+1!=args.end())
            if(0<std::stoi(*(++qubit_flag)))
                number_of_qubits = std::stoi(*qubit_flag);

    std::vector<std::string>::iterator thread_flag = std::find(args.begin(),args.end(),"-t");
    if(thread_flag!=args.end())
        if(thread_flag+1!=args.end())
            if(0<std::stoi(*(++thread_flag)))
                number_of_threads = std::stoi(*thread_flag);

    std::vector<std::string>::iterator backup_flag = std::find(args.begin(),args.end(),"-b");
    if(backup_flag!=args.end())
        if(backup_flag+1!=args.end())
            if(0<std::stoi(*(++backup_flag)))
                backup_threshold = std::stoi(*backup_flag);

    std::vector<std::string>::iterator transfer_flag = std::find(args.begin(),args.end(),"-tt");
    if(transfer_flag!=args.end())
        if(transfer_flag+1!=args.end())
            if(0<std::stoi(*(++transfer_flag)))
                transfer_threshold = std::stoi(*transfer_flag);

    if(argc >= 2)
    {
        //Read file
        const std::string input_file(argv[1]);
        std::vector<Hamiltonian*> stack = read_file(input_file,number_of_qubits);
        /*
        for(unsigned j = 0; j < stack.size(); ++j)
            for(unsigned k = 0; k < stack[j]->edges_.size(); ++k)
                std::cout << stack[j]->edges_[k].first << std::endl;

        std::cout << stack.size() << std::endl;
        */
        std::clock_t start;
        start = std::clock();

        unsigned n = split(stack,number_of_threads,backup_threshold,transfer_threshold,input_file);

        std::cout << "#####################" << std::endl;
        std::cout << "Input file: " << input_file << std::endl;
        std::cout << "Number of Hamiltonians: " << n << std::endl;
        std::cout << "CPU Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        std::cout << "#####################" << std::endl;
    }
    else
    {
        std::cout << "Please input file name" << std::endl;
    }
    return 0;
}
