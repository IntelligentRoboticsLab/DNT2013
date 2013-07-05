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
    double similarity;
    double min_similarity = DBL_MAX;
    unsigned int min_sim = 0;
    for(unsigned int i = 0; i < NUM_ANGLE_BINS; i++)
    {
        if(map.gridmap[gridCoordinates.x][gridCoordinates.y][i].valid)
        {
            std::cout << "berb" << std::endl;
            map.gridmap[gridCoordinates.x][gridCoordinates.y][i].compare(inputFeature, similarity);
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
    return (double)min_sim;
}

void WeightedExperts::checkApprCell()
{
    return;
}
