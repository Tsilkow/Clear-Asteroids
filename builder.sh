#!/bin/bash

rm -rf build/
mkdir build/
cp Carre-JWja.ttf build/Carre-JWja.ttf
cd build/
cmake ..
if eval cmake --build .; then
    cd ..
    ./build/ClearAsteroids
else
    cd ..
fi
    
