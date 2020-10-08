#!/bin/bash

rm -rf build/
mkdir build/
cp TrenchThin.ttf build/TrenchThin.ttf
cp station.png build/station.png
cd build/
cmake ..
if eval cmake --build .; then
    cd ..
    ./build/ClearAsteroids
else
    cd ..
fi
    
