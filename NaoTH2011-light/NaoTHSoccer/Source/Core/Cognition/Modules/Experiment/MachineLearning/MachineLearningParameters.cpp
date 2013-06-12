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
    PARAMETER_REGISTER(naothEvolutionParams.transmitRate) = 1.0;
    PARAMETER_REGISTER(naothEvolutionParams.crossoverRate) = 1.0;
    PARAMETER_REGISTER(naothEvolutionParams.mutationRate) = 1.0;

    PARAMETER_REGISTER(naothEvolutionParams.parentsNum) = 10;
    PARAMETER_REGISTER(naothEvolutionParams.population) = 100;
    PARAMETER_REGISTER(naothEvolutionParams.surviveNum) = 10;
    PARAMETER_REGISTER(naothEvolutionParams.maxGeneration) = 50;

    PARAMETER_REGISTER(naothEvolutionParams.resettingTime) = 3000;
    PARAMETER_REGISTER(naothEvolutionParams.standingTime) = 2000;
    PARAMETER_REGISTER(naothEvolutionParams.runningTime) = 20000;

    // Dummy method parameters
    PARAMETER_REGISTER(method2.param1) = 0.212;
    PARAMETER_REGISTER(method2.param2) = true;

    syncWithConfig();
    DebugParameterList::getInstance().add(this);
}

MachineLearningParameters::~MachineLearningParameters()
{
    DebugParameterList::getInstance().remove(this);
}
