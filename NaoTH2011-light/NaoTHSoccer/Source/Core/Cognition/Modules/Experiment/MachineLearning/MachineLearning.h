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

    //const MachineLearningParameters& getParameters() { return theParameters; }
private:
    MachineLearningParameters theParameters;
    //const MachineLearningParameters::Method1& theMethod1Parameters;
    //const MachineLearningParameters::Method2& theMethod2Parameters;

};



#endif // MACHINELEARNING_H
