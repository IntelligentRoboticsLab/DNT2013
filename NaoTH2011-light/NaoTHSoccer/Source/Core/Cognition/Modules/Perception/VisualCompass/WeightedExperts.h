#ifndef WEIGHTEDEXPERTS_H
#define WEIGHTEDEXPERTS_H

#include "Cognition/Modules/Perception/VisualCompass/VisualCompassFeature.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualCompassParameters.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualGridMapProvider.h"

class WeightedExperts
{
    public:
        WeightedExperts();
        ~WeightedExperts();
        void best_match(VisualGridMapProvider &map, VisualCompassFeature &inputFeature, double &orientation);
    private:
        void checkApprCell();

};

#endif // WEIGHTEDEXPERTS_H
