#include "hamiltonian.h"
#include <ctime>
#include <stdlib.h>

//Compile with
//g++ split_reduc.cpp algorithms.cpp hamiltonian.cpp parallel.cpp -o split_reduc -std=c++11 -fopenmp -O3

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    unsigned number_of_threads = 8;
    std::vector<std::string>::iterator thread_flag = std::find(args.begin(),args.end(),"-t");
    if(thread_flag!=args.end())
        if(thread_flag+1!=args.end())
            if(0<std::stoi(*(++thread_flag)))
                number_of_threads = std::stoi(*thread_flag);

    if(argc >= 2)
    {
        const std::string input_file(argv[1]);
        std::vector<Hamiltonian*> stack = initialize_multiple(input_file);

        std::clock_t start;
        start = std::clock();

        unsigned n = split(stack,number_of_threads);
        std::cout << "#####################" << std::endl;
        std::cout << "Input file: " << input_file << std::endl;
        std::cout << "Number of Hamiltonians: " << n << std::endl;
        std::cout << "CPU Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        std::cout << "#####################" << std::endl;
    }
    else
    {
        std::cout << "Please input 3 arguments" << std::endl;
    }
    return 0;
}
