/**
* @file IKOptimizationParameters.cpp
*
* @author <a href="mailto:wiggers.auke@gmail.com>Wiggers, Auke</a>
* Implementation of paramaters for optimization of IKMotions.
*/

#include "MachineLearningParameters.h"
#include "Tools/Debug/DebugParameterList.h"

MachineLearningParameters::MachineLearningParameters()
    : ParameterList("MachineLearningParameters")
{
    // Evolution parameters
    PARAMETER_REGISTER(evolution.transmitRate) = 1.0;
    PARAMETER_REGISTER(evolution.crossoverRate) = 1.0;
    PARAMETER_REGISTER(evolution.mutationRate) = 1.0;

    PARAMETER_REGISTER(evolution.parentsNum) = 10;
    PARAMETER_REGISTER(evolution.populationSize) = 100;
    PARAMETER_REGISTER(evolution.surviveNum) = 10;
    PARAMETER_REGISTER(evolution.maxGeneration) = 50;

    PARAMETER_REGISTER(evolution.resettingTime) = 3000;
    PARAMETER_REGISTER(evolution.standingTime) = 2000;
    PARAMETER_REGISTER(evolution.runningTime) = 10000;

    PARAMETER_REGISTER(evolution.manualReset) = false;
    PARAMETER_REGISTER(evolution.iterationsToGetUp) = 60;
    PARAMETER_REGISTER(evolution.savedirname) = "Evolution";

    PARAMETER_REGISTER(evolution.beamposition_y) = 0;
    PARAMETER_REGISTER(evolution.beamposition_x) = 0;

    // Dummy method parameters
    PARAMETER_REGISTER(method2.resettingTime) = 1000;
    PARAMETER_REGISTER(method2.standingTime) = 1000;
    PARAMETER_REGISTER(method2.runningTime) = 20000;

    PARAMETER_REGISTER(method2.manualReset) = false;
    PARAMETER_REGISTER(method2.iterationsToGetUp) = 60;
    PARAMETER_REGISTER(method2.savedirname) = "Method2";

    syncWithConfig();
    DebugParameterList::getInstance().add(this);
}

MachineLearningParameters::~MachineLearningParameters()
{
    DebugParameterList::getInstance().remove(this);
}
