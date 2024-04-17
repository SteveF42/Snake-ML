#include "XOR.hpp"
#include "math.h"

Xor::Xor() : Neat(2, 2)
{
}

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &other)
{
    for (auto i : other)
    {
        os << std::round(i * 1000) / 1000 << " ";
    }
    return os;
}

template <typename T>
double operator-(const vector<T> &rhs, const vector<T> &lhs)
{
    if (lhs.size() != rhs.size())
        throw std::invalid_argument("Vectors must be of the same size");

    double result = 0;
    for (int i = 0; i < lhs.size(); i++)
    {
        result += std::fabs(rhs[i] - lhs[i]);
    }
    return std::fabs(result);
}

double Xor::playGame(Genome *genome)
{
    auto v0 = genome->activate({0.0, 0.0});
    auto v1 = genome->activate({1.0, 0.0});
    auto v2 = genome->activate({0.0, 1.0});
    auto v3 = genome->activate({1.0, 1.0});

    double fitness = 0;
    vector<double> v0Expected = {0.0, 1.0};
    vector<double> v1Expected = {1.0, 0.0};
    vector<double> v2Expected = {1.0, 0.0};
    vector<double> v3Expected = {0.0, 1.0};

    fitness += v0 - v0Expected;
    fitness += v1 - v1Expected;
    fitness += v2 - v2Expected;
    fitness += v3 - v3Expected;

    return 1 - (fitness / 8);
}

void Xor::testNetwork(Genome *genome)
{
    auto v0 = genome->activate({0, 0});
    auto v1 = genome->activate({1, 0});
    auto v2 = genome->activate({0, 1});
    auto v3 = genome->activate({1, 1});

    double fitness = 0;

    std::cout << "(0,0) => " << v0 << '\n';
    std::cout << "(1,0) => " << v1 << '\n';
    std::cout << "(0,1) => " << v2 << '\n';
    std::cout << "(1,1) => " << v3 << '\n';
}