#include <iostream>
#include <random>
#include "Util.hpp"

int randNumber(int range)
{
    range = std::max(0,range-1);
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0,range);
    int randNum = dist(rd);
    return randNum;
}

double randDouble(double x1, double x2)
{
    std::random_device rd;
    std::uniform_real_distribution<double> dist(x1, x2);
    double randNum = dist(rd);
    return randNum;
}