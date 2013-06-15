#ifndef _GA_H
#define _GA_H

#include "MachineLearningMethod.h"
#include "GeneticAlgorithms.h"
#include "Tools/NaoInfo.h"
#include <DebugCommunication/DebugCommandManager.h>
#include "Tools/DataConversion.h"
#include "../MachineLearningParameters.h"

class GA : public MachineLearningMethod, public GeneticAlgorithms
{
public:
    GA(MachineLearningParameters::Evolution params,
       std::map<std::string, double> initialGene,
       std::map<std::string, Vector2<double> > genes);
    virtual ~GA() {}

    void update(double fitness);
    bool isFinished() const { return converged(); } // implemented by geneticalgorithms, but must be called directly
private:
    std::map<std::string, Vector2<double> > genes;
};

#endif // _GA_H
