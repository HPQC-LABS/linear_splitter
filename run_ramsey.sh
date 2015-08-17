FILES="$(find . -regex ".*/SR_H_$1_$2_.*.txt" -type f -print)"
for FILE in $FILES
do
    nohup nice -n 19 split_reduc $FILE &
done
