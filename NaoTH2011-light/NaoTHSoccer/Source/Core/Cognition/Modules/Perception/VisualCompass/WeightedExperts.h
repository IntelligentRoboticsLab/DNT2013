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

        struct output
        {
            double orientation;
            double confidence;
        };

        struct used_cell
        {
            Vector2<int> pos;
            double confidence;
        };

         vector<output> best_match(RobotPose pose, FieldInfo fInfo, VisualGridMapProvider &map, VisualCompassFeature inputFeature);
    private:
        void checkApprCell();

};

#endif // WEIGHTEDEXPERTS_H
