#ifndef WEIGHTEDEXPERTS_H
#define WEIGHTEDEXPERTS_H

#include "Cognition/Modules/Perception/VisualCompass/VisualCompassFeature.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualCompassParameters.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualGridMapProvider.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Infrastructure/FieldInfo.h"

class WeightedExperts
{
    public:
        WeightedExperts();
        ~WeightedExperts();
        double best_match(RobotPose pose, FieldInfo fInfo, VisualGridMapProvider &map, VisualCompassFeature inputFeature);
    private:
        void checkApprCell();

};

#endif // WEIGHTEDEXPERTS_H
