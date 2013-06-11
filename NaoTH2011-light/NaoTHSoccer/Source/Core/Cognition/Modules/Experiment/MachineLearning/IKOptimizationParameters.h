#ifndef IKOPTIMIZATIONPARAMETERS_H
#define IKOPTIMIZATIONPARAMETERS_H

#include <Tools/DataStructures/ParameterList.h>

class IKOptimizationParameters : public ParameterList
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
    virtual ~IKOptimizationParameters();
};

#endif // IKOPTIMIZATIONPARAMETERS_H
