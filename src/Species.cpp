#include "Species.hpp"
#include "Util.hpp"
#include <algorithm>

Genome *Species::getLeader() const
{
    return leader;
}
double Species::getScore()
{
    return score;
}

int Species::getSpeciesSize()
{
    return members.size();
}

void Species::setLeader(Genome *leader)
{
    members.push_back(leader);
    this->leader = leader;
}

void Species::setScore(double score)
{
    this->score = score;
}

void Species::addMember(Genome *member)
{
    members.push_back(member);
}

void Species::kill(double percentage)
{
    int numToKill = static_cast<int>(percentage * members.size());
    // Sort members based on fitness
    std::sort(members.begin(), members.end(), [](Genome *a, Genome *b)
              { return a->getFitness() < b->getFitness(); });
    // Remove the lowest performing genomes
    members.erase(members.begin(), members.begin() + numToKill);
}

void Species::evaluateScore()
{
    double total = 0;
    for (Genome *g : members)
    {
        total += g->getFitness();
    }
    score = total / members.size();
}

Genome *Species::breed()
{
    int size = members.size();
    Genome *g1 = members[size - 1];
    Genome *g2 = members[randNumber(size)];

    if (g1->getFitness() > g2->getFitness())
        return Genome::crossGenomes(*g1, *g2);
    else
        return Genome::crossGenomes(*g2, *g1);
}

void Species::clear()
{
    members.clear();
}