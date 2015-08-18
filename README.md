Parallel Splitter
===============
Installation
------------
First download the source files using 

    git clone https://github.com/emileokada/linear_splitter.git

Then compile using

    ./compile.sh

Notes on compilation flags:
- `std=c++11` allow use of latest C++ standards
- `O3` is the highest level of optimization. `oFast` doesn't seem to make a difference
- `fopenmp` enables parallelization

Usage
-----
Splitting can be used on any hamiltonian in the correct format. Simply run 

    ./split_reduc input_file_name

To generate Ramsey number hamiltonians run

    MathematicaScript -script generate_hamiltonian.h m n N

Where m is the number of cliques, n is the number of independent sets and N is the number of vertices.

If you want to create Ramsey number hamiltonians with search space reduction run

    MathematicaScript -script sr_hamiltonian.h m n N

Note that `MathematicaScript` needs to be in your path. 

You can also create a batch of hamiltonains by running

    ./create_hamiltonians.sh m n N

It will generate the files from 6 up to N. Use

    ./run_hamiltonians.sh m n

to run all hamiltonians associated with R(m,n).

Runtime flags
-------------
- To change the number of threads used for splitting supply the `-t` flag **after** the input file and then number of threads e.g. `./split_reduc input_file -t 8`.

Known issues
------------
Sometimes `MathematicaScript` is installed as `MathematicaScript10`. To fix, replace all instances of `MathematicaScript` with `MathematicaScript10` (this also includes the create_hamiltonians.sh file).

You might have to create a directory in this repo called `hamiltonians`.
