#include "randoms.hpp"


#include <stdlib.h>
#include <math.h>


int randomI(int min, int max)
{
    if(min == max) return min;
    return rand() % (max - min + 1) + min;
}

float randomF(float min, float max, float step)
{
    return randomI(std::round(min * 1.f/step), std::round(max * 1.f/step)) * step;
}
