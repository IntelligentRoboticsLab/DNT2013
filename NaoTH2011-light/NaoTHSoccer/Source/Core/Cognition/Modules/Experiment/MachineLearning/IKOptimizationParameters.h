#ifndef IKOPTIMIZATIONPARAMETERS_H
#define IKOPTIMIZATIONPARAMETERS_H

#include <Tools/DataStructures/ParameterList.h>

class IKOptimizationParameters
{
public:
    struct Method1 {
        double param1;
        double param2;
    } method1;

    struct Method2 {
        double param1;
        bool param2;
    } method2;

    IKOptimizationParameters();
    ~IKOptimizationParameters();
};

#endif // IKOPTIMIZATIONPARAMETERS_H
