#include "hamiltonian.h"
#include <ctime>

//Compile with
//g++ split_reduc.cpp algorithms.cpp hamiltonian.cpp -o split_reduc -std=c++11

int main(int argc, char* argv[]){
    
    if(argc >=4)
    {
        std::istringstream streamm( argv[1] ), streamn( argv[2] ), streamN( argv[3] );
        int m, n, N;

        if ((streamm >> m)&&(streamn >> n)&&(streamN >> N)&&(N>=m)&&(N>=n))
        {
            Hamiltonian* root = new Hamiltonian(m,n,N);

            std::clock_t start;
            start = std::clock();

            unsigned n = split(root);
            std::cout << "Number of Hamiltonians: " << n << std::endl;

            std::cout << "CPU Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
        }
    }
    else
    {
        std::cout << "Please input 3 arguments" << std::endl;
    }
    return 0;
}
