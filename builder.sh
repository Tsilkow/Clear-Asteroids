#!/bin/bash

rm -rf build/
mkdir build/
cd build/
cmake ..
if eval cmake --build .; then
    cd ..
    ./build/ClearAsteroids
else
    cd ..
fi
    
