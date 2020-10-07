#pragma once

#include <iostream>

int randomI(int min, int max);
// random integer with target - n numbers generated within range and then one closest to target is chosen
int randomIwT(int min, int max, int target, int attempts);
float randomF(float min, float max, float step = 0.01);
float randomFwT(float min, float max, float target, int attempts, float step = 0.01);
