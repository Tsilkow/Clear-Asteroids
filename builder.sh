#!/bin/bash

#rm -rf build/
#mkdir build/
#cp Sicretmono.ttf build/Sicretmono.ttf
#cp station.png build/station.png
#cp scores.json build/scores.json
cd build/
cmake -DCMAKE_BUILD_TYPE=Debug ..
if eval cmake --build .; then
    ./ClearAsteroids
    cd ..
else
    cd ..
fi
    
