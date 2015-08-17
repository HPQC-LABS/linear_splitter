START=6
END=$3
for ((N=$START; N<=$END; N++))
do
    MathematicaScript10 -script sr_hamiltonian.m $1 $2 $N
done
