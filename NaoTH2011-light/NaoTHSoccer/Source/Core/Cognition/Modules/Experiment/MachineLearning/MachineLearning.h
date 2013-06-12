#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H

#include <ModuleFramework/Module.h>

#include <Tools/DataStructures/ParameterList.h>
#include "Representations/Motion/Request/MotionRequest.h"
#include "MachineLearningParameters.h"

BEGIN_DECLARE_MODULE(MachineLearning)
    PROVIDE(MotionRequest)
END_DECLARE_MODULE(MachineLearning)

class MachineLearning : public MachineLearningBase
{

public:
    MachineLearning();
    ~MachineLearning();
    virtual void execute();

    //const IKOptimizationParameters& getParameters() { return theParameters; }
private:
    IKOptimizationParameters theParameters;
    //const IKOptimizationParameters::Method1& theMethod1Parameters;
    //const IKOptimizationParameters::Method2& theMethod2Parameters;

};



#endif // MACHINELEARNING_H
