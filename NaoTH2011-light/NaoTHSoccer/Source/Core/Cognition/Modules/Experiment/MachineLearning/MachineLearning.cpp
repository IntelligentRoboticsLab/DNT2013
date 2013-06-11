#include "MachineLearning.h"

MachineLearning::MachineLearning()
{
    theParameters.method1.param1 = 0.1;
    //theMethod1Parameters(theParameters.method1);
    //theMethod2Parameters(theParameters.method2);
}

MachineLearning::~MachineLearning() {
    theParameters.method1.param1 = 0.0000;
}

void MachineLearning::execute() {
    theParameters.method1.param1 = 0.99999;
}

