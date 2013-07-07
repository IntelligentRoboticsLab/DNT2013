#include "WeightedExperts.h"

WeightedExperts::WeightedExperts()
{
}

WeightedExperts::~WeightedExperts()
{
}

vector<WeightedExperts::output> WeightedExperts::best_match(RobotPose pose, FieldInfo fInfo, VisualGridMapProvider &map, VisualCompassFeature inputFeature)
{
    Vector3<int> gridCoordinates = map.fieldPosToGridPos(pose, fInfo);
    long double similarity;
    long double min_similarity = DBL_MAX;
    vector<output> temp;

    for(unsigned int i = 0; i < NUM_ANGLE_BINS; i++)
    {
        if(map.gridmap[gridCoordinates.x][gridCoordinates.y][i].valid)
        {
            map.gridmap[gridCoordinates.x][gridCoordinates.y][i].compare(inputFeature, similarity);
            output a;
            a.orientation = map.gridmap[gridCoordinates.x][gridCoordinates.y][i].orientation;
            a.confidence = similarity;
            temp.push_back(a);
        }
        else
        {
            similarity = DBL_MAX;
        }
        if(similarity < min_similarity)
        {
            min_similarity = similarity;     
        }
    }
    return temp;
}

void WeightedExperts::checkApprCell()
{
    return;
}
