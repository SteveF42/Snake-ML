#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include "Util.hpp"

using std::ifstream;
using std::string;

int randNumber(int range)
{
    range = std::max(0, range - 1);
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, range);
    int randNum = dist(rd);
    return randNum;
}

double randDouble(double x1, double x2)
{
    std::random_device rd;
    std::uniform_real_distribution<double> dist(x1, x2);
    double randNum = dist(rd);
    return randNum;
}

void Config::initialize()
{
    std::filesystem::path path = std::filesystem::current_path() / "config.txt";
    ifstream file(path.string());
    if (!file.is_open())
    {
        std::cerr << "Error opening config file\n";
        exit(1);
    }

    string line;
    while (std::getline(file, line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
        if (line[0] == '#' || line.empty())
        {
            continue;
        }

        auto delimiter = line.find('=');
        string name = line.substr(0, delimiter);
        double value = std::stod(line.substr(delimiter + 1));
        assignValue(name, value);
    }
}

void Config::assignValue(const std::string &name, double value)
{
    // BuT wHy nOt UsE a SwItCh StAtEmEnT?
    // shut up the compiler is smarter than you it'll optimize it to a switch statement
    // plus switch statements don't work with strings
    if(name == "min")
    {
        min = value;
    }
    else if(name == "max")
    {
        max = value;
    }
    else if(name == "deathRate")
    {
        deathRate = value;
    }
    else if(name == "excessCoefficient")
    {
        excessCoefficient = value;
    }
    else if(name == "disjointCoefficient")
    {
        disjointCoefficient = value;
    }
    else if(name == "weightDiffCoefficient")
    {
        weightDiffCoefficient = value;
    }
    else if(name == "weightShiftStrength")
    {
        weightShiftStrength = value;
    }
    else if(name == "mutateAddLinkProbability")
    {
        mutateAddLinkProbability = value;
    }
    else if(name == "mutateRemoveLinkProbability")
    {
        mutateRemoveLinkProbability = value;
    }
    else if(name == "mutateAddNodeProbability")
    {
        mutateAddNodeProbability = value;
    }
    else if(name == "mutateRemoveNodeProbability")
    {
        mutateRemoveNodeProbability = value;
    }
    else if(name == "mutateWeightShiftProbability")
    {
        mutateWeightShiftProbability = value;
    }
    else if(name == "mutateWeightRandomProbability")
    {
        mutateWeightRandomProbability = value;
    }
    else if(name == "mutateEnableLinkProbability")
    {
        mutateEnableLinkProbability = value;
    }
    else if(name == "speciationThreshold")
    {
        speciationThreshold = value;
    }
    else if(name == "mutateBiasShiftProbability")
    {
        mutateBiasShiftProbability = value;
    }
    else if(name == "initialLinkProbability")
    {
        initialLinkProbability = value;
    }
    else if(name == "complexityPenalty")
    {
        complexityPenalty = value;
    }
    else if(name == "mutationRate")
    {
        mutationRate = value;
    }
    else if(name == "topPerformerPercentage")
    {
        topPerformerPercentage = value;
    }
    else
    {
        std::cerr << "Invalid config name\n";
        exit(1);
    }
}