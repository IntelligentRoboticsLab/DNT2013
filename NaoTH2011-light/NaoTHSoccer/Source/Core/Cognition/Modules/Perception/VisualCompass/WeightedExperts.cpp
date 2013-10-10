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

//    if(COMPASS_PARTICLE_UPDATE)
//    {
//        std::vector< Vector2<int> > cells;
//        double max_conf = DBL_MIN;
//        for (int i = 0; i < GRID_X_LENGTH; ++i)
//        {
//            for (int j = 0; j < GRID_Y_LENGTH; ++j)
//            {
//                if(map.gridCellConfidence[i][j] > 0.20) cells.push_back(Vector2<int>(i, j));
//                if(map.gridCellConfidence[i][j] > max_conf) max_conf = map.gridCellConfidence[i][j];
//            }
//        }
//        if(!cells.empty())
//        {
//            for(unsigned int i = 0; i < NUM_ANGLE_BINS; i++)
//            {
//                double sum_similarity = 0.00;
//                for(unsigned int j = 0; j < cells.size(); j++)
//                {
//                    if(map.gridmap[cells.at(j).x][cells.at(j).y][i].valid)
//                    {
//                        map.gridmap[cells.at(j).x][cells.at(j).y][i].compare(inputFeature, similarity);
//                        similarity *= (1 - map.gridCellConfidence[cells.at(j).x][cells.at(j).y]);
//                        sum_similarity += similarity;
//                    }
//                }
//                output a;
//                a.orientation = i * (NUM_ANGLE_BINS/360);
//                a.confidence = sum_similarity;
//                temp.push_back(a);
//            }
//        }
//        else
//        {
//            // do nothing, particles all over the field, we cant do anything.
//        }
//        return temp;
//    }
//    else
//    {
        double min_sim = DBL_MAX;
        double orientation = 0.00;
        for(unsigned int i = 0; i < NUM_ANGLE_BINS; i++)
        {
            if(map.gridmap[gridCoordinates.x][gridCoordinates.y][i].valid)
            {
                map.gridmap[gridCoordinates.x][gridCoordinates.y][i].compare(inputFeature, similarity);
                // multiplied by 1-conf, cause we have min similarity, less means better.
                similarity *= 1 - map.gridCellConfidence[gridCoordinates.x][gridCoordinates.y];
                if(similarity < min_sim)
                {
                    min_sim = similarity;
                    orientation = map.gridmap[gridCoordinates.x][gridCoordinates.y][i].orientation;
                }
            }
            
        }
        output a;
        a.orientation = orientation;
        a.confidence = min_sim;
        temp.push_back(a);
//    }
    return temp;
}

void WeightedExperts::checkApprCell()
{
    return;
}
