#ifndef WEIGHTEDEXPERTS_H
#define WEIGHTEDEXPERTS_H

#include "Cognition/Modules/Perception/VisualCompass/VisualCompassFeature.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualCompassParameters.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualGridMapProvider.h"
#include "Representations/Modeling/RobotPose.h"

class WeightedExperts
{
    public:
        WeightedExperts();
        ~WeightedExperts();
        void best_match(RobotPose pose, VisualGridMapProvider &map, VisualCompassFeature &inputFeature, double &orientation);
    private:
        void checkApprCell();

};

#endif // WEIGHTEDEXPERTS_H
