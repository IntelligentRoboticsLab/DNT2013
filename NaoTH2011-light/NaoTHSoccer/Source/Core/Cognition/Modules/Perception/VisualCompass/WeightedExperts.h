#ifndef WEIGHTEDEXPERTS_H
#define WEIGHTEDEXPERTS_H

#include "VisualCompassFeature.h"
#include "VisualCompassParameters.h"
#include "VisualGridMapProvider.h"

class WeightedExperts
{
    public:
		WeightedExperts (); 
        void best_match (VisualGridMapProvider &grid, vector<VisualCompassFeature> input, double &out);
};

#endif // WEIGHTEDEXPERTS_H
