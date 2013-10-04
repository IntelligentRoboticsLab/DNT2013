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
    vector<output> temp;


    if( map.gridCellConfidence[gridCoordinates.x][gridCoordinates.y] >= 0.51 )
    {

    }


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
    }
    return temp;
}

void WeightedExperts::checkApprCell()
{
    return;
}
