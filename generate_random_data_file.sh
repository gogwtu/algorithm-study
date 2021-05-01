#!/bin/bash
#./generate_random_data_file.sh file_path data_size

file_path=$1
data_size=$2

echo "@file_path=$file_path,data_size=$data_size"
#create empty-file
echo -n " " > $file_path
i=1
#fill file with random-numbers
while [ $i -le $data_size ]
do
  rand_number=$(($RANDOM % $data_size))
  echo -n $rand_number" " >> $file_path
  i=$(($i + 1))
done
