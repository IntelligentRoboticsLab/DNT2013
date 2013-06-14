#ifndef _GA_H
#define _GA_H

#include "MachineLearningMethod.h"
#include "GeneticAlgorithms.h"
#include <math.h>

class GA : public MachineLearningMethod, public GeneticAlgorithms
{
public:
    GA();
    ~GA();

    void update(double fitness);
    bool isFinished() {return false;}
    void run() = 0;
};

#endif // _GA_H
