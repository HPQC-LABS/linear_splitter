Parallel Splitter
===============
Use generate_hamiltonian.m to generate the original Ramsey hamiltonian. Run using `MathematicaScript -script generate_hamiltonian.m m n N` where `m`, `n` and `N` are the standard Ramsey inputs.
To split use `./split_reduc m n N`. The output is the number of hamiltonians and CPU time.

Installation
------------
First download the source files using 

    git clone https://github.com/emileokada/linear_splitter.git

Then compile using

    g++ split_reduc.cpp algorithms.cpp hamiltonian.cpp parallel.cpp -o split_reduc -std=c++11 -fopenmp -O3

Notes on compilation flags:
- `std=c++11` allow use of latest C++ standards
- `O3` is the highest level of optimization. `oFast` doesn't seem to make a difference
- `fopenmp` enables parallelization

Usage
-----
Splitting can be used on any hamiltonian in the correct format. Simply run 

    ./split_reduc inpul_file_name

To also generate Ramsey number hamiltonians run

    MathematicaScript -script generate_hamiltonian.h m n N

Where m is the number of cliques, n is the number of independent sets and N is the number of vertices.
If create Ramsey number hamiltonians with search space reduction run
    MathematicaScript -script sr_hamiltonian.h m n N

Note that `MathematicaScript` needs to be in your path. 

Known issues
------------
Sometimes `MathematicaScript` is installed as `MathematicaScript10`. To fix, replace all instances of `MathematicaScript` with `MathematicaScript10`.
