#ifndef _GA_H
#define _GA_H

#include "MachineLearningMethod.h"
#include "GeneticAlgorithms.h"
#include <math.h>
#include <iostream>
#include <vector>
#include "Tools/Math/Common.h"
#include "Tools/NaoInfo.h"

class GA : public MachineLearningMethod, public GeneticAlgorithms
{
public:
    GA();
    ~GA();

    void update(double fitness);
    virtual bool isFinished() const; // implemented by geneticalgorithms
private:
    std::map<std::string, Vector2<double> > genes;
};

#endif // _GA_H
