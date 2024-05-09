#include "Neat.hpp"
#include "Util.hpp"

using std::make_unique;

Neat::Neat(int input, int output, int hidden)
{
    this->input = input;
    this->output = output;
    this->hidden = hidden;
    this->population = 0;
}

void Neat::initialize(int population)
{
    this->population = population;
    Config::initialize();
    LinkGene::setNextID(hidden > 0 ? hidden * (input + output) : input * output);
    NodeGene::setNextID(hidden > 0 ? input + output + hidden : input + output);
    for (int i = 0; i < population; i++)
    {
        allGenomes.push_back(make_unique<Genome>(input, output, true, hidden));
    }
}

void Neat::train(int genCount)
{
    bestGenome.setFitness(INT32_MIN);
    for (int i = 0; i < genCount; i++)
    {
        this->genCount++;
        trainGeneration();
        evolve();
    }
}

void Neat::test()
{
    testNetwork(&bestGenome);
}

const vector<GenomePtr> &Neat::getGenomes() const
{
    return allGenomes;
}

const vector<SpeciesPtr> &Neat::getSpecies() const
{
    return allSpecies;
}

Genome Neat::getBestGenome()
{
    std::shared_lock lock(mutex_);
    return bestGenome;
}

int Neat::getPopulation() const
{
    return population;
}

std::ostream &operator<<(std::ostream &os, const Neat &other)
{
    os << "Population: " << other.population << '\n';
    os << "Genomes: " << other.allGenomes.size() << '\n';
    for (const auto &species : other.allSpecies)
    {
        os << "Species size: ";
        os << other.allSpecies.size() << '\n';
    }
    return os;
}

void Neat::trainGeneration()
{
    avgFitness = 0;
    for (const auto &genome : allGenomes)
    {
        genome->setFitness(playGame(genome.get()));
        avgFitness += genome->getFitness();
        std::unique_lock lock(mutex_);
        if (genome->getFitness() > bestGenome.getFitness())
        {
            bestGenome = *genome;
        }
    }
    avgFitness /= population;
    std::cout << "Generation: " << genCount
              << " Population: " << allGenomes.size()
              << " Average Fitness: " << avgFitness
              << " Best Fitness: " << bestGenome.getFitness()
              << " Species Count: " << allSpecies.size() << '\n';
}

void Neat::evolve()
{
    speciate();
    kill();
    breed();
    mutate();
}

void Neat::speciate()
{
    allSpecies.clear();
    for (const auto &genome : allGenomes)
    {
        bool found = false;
        for (const auto &species : allSpecies)
        {
            const Genome *leader = species->getLeader();
            int distance = genome->distance(*leader);
            if (distance <= Config::speciationThreshold)
            {
                species->addMember(genome.get());
                found = true;
                break;
            }
        }
        if (!found)
        {
            SpeciesPtr newSpecies = make_unique<Species>();
            newSpecies->setLeader(genome.get());
            allSpecies.push_back(std::move(newSpecies));
        }
    }

    double adjAverage = 0;
    for (const auto &species : allSpecies)
    {
        species->evaluateScore();
        adjAverage += species->getScore();
    }
    adjAverage /= allSpecies.size();
    for (const auto &species : allSpecies)
    {
        species->setNumOfChildren((species->getScore() / adjAverage) * species->getSpeciesSize());
    }
}

void Neat::kill()
{
    for (auto it = allSpecies.begin(); it != allSpecies.end();)
    {
        Species *species = it->get();
        species->kill(Config::deathRate);
        if (species->getSpeciesSize() < 2)
        {
            it = allSpecies.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Neat::breed()
{
    vector<GenomePtr> newGenomes;
    for (const auto &species : allSpecies)
    {
        // keep the top performers
        int numElites = static_cast<int>(Config::topPerformerPercentage * species->getSpeciesSize());
        for (int i = 0; i < numElites && i < species->getNumOfChildren(); i++)
        {
            GenomePtr elite = make_unique<Genome>(*species->getMember(i));
            elite->child = false;
            newGenomes.push_back(std::move(elite));
        }

        // breed the rest
        for (int i = numElites; i < species->getNumOfChildren(); i++)
        {
            GenomePtr child = GenomePtr(species->breed());
            child->child = true;
            newGenomes.push_back(std::move(child));
        }
    }

    // idk in case something stupid happens
    for (int i = newGenomes.size(); i < population; i++)
    {
        int index = randNumber(allSpecies.size());
        GenomePtr child = GenomePtr(allSpecies[index]->breed());
        child->child = true;
        newGenomes.push_back(std::move(child));
    }
    allGenomes.clear();
    this->allGenomes = std::move(newGenomes);
}

void Neat::mutate()
{
    for (const auto &genome : allGenomes)
    {

        if (randDouble(0, 1) <= Config::mutationRate && genome->child)
            genome->mutate();
    }
}