#ifndef UTIL_HPP
#define UTIL_HPP
#include<cstdlib>
#include<time.h>

int randNumber(int range);

double randDouble(double x1, double x2);

struct Config
{
public:
    static void initialize();

    inline static double min = -10;
    inline static double max = 10;
    inline static double deathRate = 0.7;
    inline static double excessCoefficient = 0.50;
    inline static double disjointCoefficient = 0.50;
    inline static double weightDiffCoefficient = 0.20;
    inline static double weightShiftStrength = 1;
    inline static double mutateAddLinkProbability = 0.20;
    inline static double mutateRemoveLinkProbability = 0.20;
    inline static double mutateAddNodeProbability = 0.15;
    inline static double mutateRemoveNodeProbability = 0.15;
    inline static double mutateWeightShiftProbability = 0.80;
    inline static double mutateWeightRandomProbability = 0;
    inline static double mutateEnableLinkProbability = 0.10;
    inline static double speciationThreshold = 1.4;
    inline static double mutateBiasShiftProbability = 0.8;
    inline static double initialLinkProbability = 1;
    inline static double complexityPenalty = 0.00001;
    inline static double mutationRate = 1;
    inline static double topPerformerPercentage = 0.2;
private:
    static void assignValue(const std::string &name, double value);
};

#endif