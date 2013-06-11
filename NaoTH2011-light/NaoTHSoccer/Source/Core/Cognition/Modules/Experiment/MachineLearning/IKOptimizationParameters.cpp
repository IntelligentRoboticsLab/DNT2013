/**
* @file IKOptimizationParameters.cpp
*
* @author <a href="mailto:wiggers.auke@gmail.com>Wiggers, Auke</a>
* Implementation of paramaters for optimization of IKMotions.
*/

#include "IKOptimizationParameters.h"
#include "Tools/Debug/DebugParameterList.h"

IKOptimizationParameters::IKOptimizationParameters()
    : ParameterList("IKOptimizationParameters")
{
    PARAMETER_REGISTER(method1.param1) = 0.9;
    PARAMETER_REGISTER(method1.param2) = 0.2;

    PARAMETER_REGISTER(method2.param1) = 0.212;
    PARAMETER_REGISTER(method2.param2) = true;

    syncWithConfig();
    DebugParameterList::getInstance().add(this);
}

IKOptimizationParameters::~IKOptimizationParameters()
{
    DebugParameterList::getInstance().remove(this);
}
