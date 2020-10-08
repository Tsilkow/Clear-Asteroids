#!/bin/bash

rm -rf build/
mkdir build/
cp TrenchThin.ttf build/TrenchThin.ttf
cd build/
cmake ..
if eval cmake --build .; then
    cd ..
    ./build/ClearAsteroids
else
    cd ..
fi
    
