#!/bin/bash

cmake -S . -B build/ -G "Ninja"
cmake --build build/ --target Reconstruction

#Check if the compilation was successful
if [ $? -eq 0 ]; then
	echo "Compilation successful. Running the programm..."
	./build/Reconstruction
else
    echo "Compilation failed. Please check the error messages."
fi