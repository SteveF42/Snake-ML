#include "ActivationRelu.hpp"

double ActivationRelu::activate(double x)
{
    if (x < 0)
    {
        return 0;
    }
    return x;
}


double ActivationRelu::getOutput() const
{
    return output;
}

void ActivationRelu::setActivation(double x)
{
    output = x;
}