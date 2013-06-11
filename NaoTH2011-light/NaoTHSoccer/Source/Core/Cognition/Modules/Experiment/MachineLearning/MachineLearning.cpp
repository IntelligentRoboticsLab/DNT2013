#include "MachineLearning.h"

MachineLearning::MachineLearning()
{
    theParameters = new IKOptimizationParameters();
    //theMethod1Parameters(theParameters.method1);
    //theMethod2Parameters(theParameters.method2);
}

MachineLearning::~MachineLearning() {
    theParameters.method1.param1 = 0.0000;
}

MachineLearning::execute() {
    theParameters.method1.param1 = 0.99999;
}

