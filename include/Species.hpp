#ifndef Species_HPP
#define Species_HPP
#include <vector>
#include "Genome.hpp"
using std::vector;

class Species
{
public:
    Genome* getLeader() const;
    double getScore();
    int getSpeciesSize();
    vector<Genome*> getMembers() const;

    void setLeader(Genome* leader);
    void setScore(double score);
    
    void addMember(Genome* member);
    void kill(double percentage);
    void evaluateScore();
    Genome* breed();

    void clear();

    void setNumOfChildren(int num);
    int getNumOfChildren();
private:
    Genome* leader;
    double score;
    int numOfChildren;
    vector<Genome*> members;
};

#endif