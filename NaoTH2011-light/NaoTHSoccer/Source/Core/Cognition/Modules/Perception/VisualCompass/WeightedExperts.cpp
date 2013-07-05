#include "WeightedExperts.h"

WeightedExperts::WeightedExperts()
{
}

WeightedExperts::~WeightedExperts()
{
}

double WeightedExperts::best_match(RobotPose pose, FieldInfo fInfo, VisualGridMapProvider &map, VisualCompassFeature inputFeature)
{
    Vector3<int> gridCoordinates = map.fieldPosToGridPos(pose, fInfo);
    long double similarity;
    long double min_similarity = DBL_MAX;
    unsigned int min_sim = 0;
    for(unsigned int i = 0; i < NUM_ANGLE_BINS; i++)
    {
        if(map.gridmap[gridCoordinates.x][gridCoordinates.y][i].valid)
        {
            map.gridmap[gridCoordinates.x][gridCoordinates.y][i].compare(inputFeature, similarity);
            std::cout << similarity << std::endl;
        }
        else
        {
            similarity = DBL_MAX;
        }
        if(similarity < min_similarity)
        {
            min_similarity = similarity;
            min_sim = i;
        }
    }
    return (double)min_sim*2;
}

void WeightedExperts::checkApprCell()
{
    return;
}
