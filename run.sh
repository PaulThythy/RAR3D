#!/bin/bash

build_type=Debug

cmake -S . -B bin/$build_type -G "Ninja" -DCMAKE_BUILD_TYPE="$build_type"
cmake --build bin/$build_type --target Reconstruction

#Check if the compilation was successful
if [ $? -eq 0 ]; then
	echo "Compilation successful. Running the programm..."
	#Execute the raytracer binary
	./bin/$build_type/Reconstruction
else
    echo "Compilation failed. Please check the error messages."
fi