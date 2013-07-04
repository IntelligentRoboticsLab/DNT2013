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

    int parentsNum;
    int populationSize;
    int surviveNum;
    int maxGeneration;

    unsigned int resettingTime;
    unsigned int standingTime;
    unsigned int runningTime;

    bool manualReset;
    unsigned int iterationsToGetUp;

    std::string savedirname;

    int beamposition_x;
    int beamposition_y;

  } evolution;

  struct Method2 {
    bool manualReset;
    unsigned int iterationsToGetUp;

    unsigned int resettingTime;
    unsigned int standingTime;
    unsigned int runningTime;
    std::string savedirname;
  } method2;

  MachineLearningParameters();
  virtual ~MachineLearningParameters();
};

#endif // IKOPTIMIZATIONPARAMETERS_H
