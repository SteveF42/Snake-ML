#ifndef Activation_HPP
#define Activation_HPP

class ActivationRelu
{
public:
    double activate(double x);
    double getOutput() const;
    void setActivation(double x);

private:
    double output;
};

class ActivationSigmoid
{
public:
    double activate(double x);
    double getOutput() const;

private:
    double output;
};

#endif