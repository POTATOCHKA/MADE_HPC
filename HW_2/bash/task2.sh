#!/bin/bash
arr=(kek1 "kek2" kek perekek "kek_perekek" 22 44 1 2 3 4 5 6 7 8)

for i in ${arr[@]}
do
  echo ${i}
done
# using indexes
for index in ${!arr[@]}
do
    echo ${arr[$index]}
done
