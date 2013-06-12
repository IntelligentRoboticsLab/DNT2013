#ifndef IKOPTIMIZATIONPARAMETERS_H
#define IKOPTIMIZATIONPARAMETERS_H

#include <Tools/DataStructures/ParameterList.h>

class MachineLearningParameters : public ParameterList
{
public:
    struct Evolution {
        double transmitRate;
        double crossoverRate;
        double mutationRate;

        int parentsNum; //(10),
        int population; //(100),
        int surviveNum; //(10),
        int maxGeneration; //(50),

        unsigned int resettingTime; // = 3000;
        unsigned int standingTime; // = 2000;
        unsigned int runningTime; // = 20000;
    } naothEvolutionParams;

    struct Method2 {
        double param1;
        bool param2;
    } method2;

    MachineLearningParameters();
    virtual ~MachineLearningParameters();
};

#endif // IKOPTIMIZATIONPARAMETERS_H
