#include "hamiltonian.h"
#include <ctime>
#include <stdlib.h>

//Compile with
//g++ split_reduc.cpp algorithms.cpp hamiltonian.cpp parallel.cpp -o split_reduc -std=c++11 -fopenmp -O3

int main(int argc, char* argv[])
{
    if(argc >= 4)
    {
        std::istringstream streamm( argv[1] ), streamn( argv[2] ), streamN( argv[3] );
        int m, n, N;

        if ((streamm >> m)&&(streamn >> n)&&(streamN >> N)&&(N>=m)&&(N>=n))
        {
            std::vector<Hamiltonian*> stack(1, new Hamiltonian(m,n,N));

            std::clock_t start;
            start = std::clock();

            unsigned nH = split(stack);
            std::cout << "#####################" << std::endl;
            std::cout << "R(" << m << "," << n << "," << N << ") - number of Hamiltonians: " << nH << std::endl;
            std::cout << "CPU Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
            std::cout << "#####################" << std::endl;
        }
    }
    else if(argc == 2)
    {
        const std::string input_file(argv[1]);
        std::vector<Hamiltonian*> stack = initialize_multiple(input_file);

        std::clock_t start;
        start = std::clock();

        unsigned n = split(stack);
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
