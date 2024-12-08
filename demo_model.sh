#!/bin/bash

clear

data_path=$3
validating_data_path=$4

saving_file=$2
epochs=$1

if [ -f result ]; then
    rm result
fi

touch result

echo "Compiling the demo model..."
sleep 2

make -C ./alphabet-training clean
make -C ./alphabet-training demo

clear

echo "Running the demo model..."
sleep 1

./alphabet-training/training $data_path $saving_file $epochs

echo "Training completed."
sleep 2


echo "Compiling the validation model..."
sleep 2

make -C ./network clean
make -C ./network demo

clear

echo "Running the validation model..."
sleep 1

./network/network $saving_file $validating_data_path