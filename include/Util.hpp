#ifndef UTIL_HPP
#define UTIL_HPP
#include<cstdlib>
#include<time.h>

int randNumber(int range);

double randDouble(double x1, double x2);

struct Config
{
    inline const static double min = -20;
    inline const static double max = 20;
    inline const static double deathRate = 0.7;
    inline const static double excessCoefficient = 0.50;
    inline const static double disjointCoefficient = 0.50;
    inline const static double weightDiffCoefficient = 0.15;
    inline const static double weightShiftStrength = 0.90;
    inline const static double mutateAddLinkProbability = 0.15;
    inline const static double mutateRemoveLinkProbability = 0.15;
    inline const static double mutateAddNodeProbability = 0.10;
    inline const static double mutateRemoveNodeProbability = 0.10;
    inline const static double mutateWeightShiftProbability = 0.70;
    inline const static double mutateWeightRandomProbability = 0;
    inline const static double mutateEnableLinkProbability = 0.10;
    inline const static double speciationThreshold = 1.4;
    inline const static double mutateBiasShiftProbability = 0.50;
    inline const static double complexityPenalty = 2;
    inline const static double mutationRate = 1;
};

#endif