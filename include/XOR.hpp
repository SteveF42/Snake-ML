#ifndef XOR_HPP
#define XOR_HPP
#include "Neat.hpp"

class Xor : public Neat
{
public:
    Xor();

protected:
    double playGame(Genome *genome) override;
    void testNetwork(Genome *genome) override;
};

#endif