#include "randoms.hpp"


#include <stdlib.h>
#include <math.h>


int randomI(int min, int max)
{
    if(min == max) return min;
    return rand() % (max - min + 1) + min;
}

int randomIwT(int min, int max, int target, int attempts)
{
    int result = randomI(min, max);
    
    for(int i = 1; i < attempts; ++i)
    {
	int attempt = randomI(min, max);
	if(std::abs(result - target) > std::abs(attempt - target)) result = attempt;
    }

    return result;
}

float randomF(float min, float max, float step)
{
    return randomI(std::round(min * 1.f/step), std::round(max * 1.f/step)) * step;
}

float randomFwT(float min, float max, float target, int attempts, float step)
{
    return randomIwT(std::round(min    * 1.f/step),
		     std::round(max    * 1.f/step),
		     std::round(target * 1.f/step), attempts) * step;
}
