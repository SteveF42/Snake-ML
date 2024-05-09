#ifndef Neat_HPP
#define Neat_HPP
#include "Genome.hpp"
#include "Species.hpp"
#include <iostream>
#include <atomic>
#include <memory>
#include <shared_mutex>
#include <mutex>

using std::unique_ptr;
using std::atomic;
using std::shared_mutex;

typedef unique_ptr<Genome> GenomePtr;
typedef unique_ptr<Species> SpeciesPtr;

class Neat
{
public:
    Neat(int input, int output, int hidden = 0);
    void initialize(int population = 500);
    void train(int genCount);
    void evolve();
    void clear();
    void test();

    const vector<GenomePtr> &getGenomes() const;
    const vector<SpeciesPtr> &getSpecies() const;
    Genome getBestGenome();
    int getPopulation() const;
    friend std::ostream &operator<<(std::ostream &os, const Neat &other);

protected:
    int input;
    int output;
    int hidden;
    int population;
    int genCount = 0;
    double avgFitness = 0;
    Genome bestGenome;
    vector<GenomePtr> allGenomes;
    vector<SpeciesPtr> allSpecies;
    // inherited class will implement playGame
    virtual double playGame(Genome *genome) = 0;
    virtual void testNetwork(Genome *genome) = 0;
    void trainGeneration();

private:
    shared_mutex mutex_;
    void speciate();
    void kill();
    void breed();
    void mutate();
};

#endif
