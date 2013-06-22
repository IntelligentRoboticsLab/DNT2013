#include "WeightedExperts.h"

using namespace std;

WeightedExperts::WeightedExperts () {
	//map of x,y,angle bins
	//VisualGridMapProvider map = new VisualGridProvider();
	//double angle_size = 2*M_PI/(double)AngleBins;
}

void WeightedExperts::best_match (VisualGridMapProvider &grid, vector<VisualCompassFeature> input, double &out) {
	//find grid coords
	VisualCompassFeature first_elem = input[0];
	Vector2<int> input_loc;
	VisualGridMapProvider::fieldPosToGridPos(input[0].source_position, input_loc);
	//search in the grid for a good enough/best match
	//VisualCompassFeature[] features = map[input_loc.x][input_loc.y];
	double best_similarity = 0.0;
	for (int i = 0; i < AngleBins; i++)
    {
		double tot_similarity = 0.0;
		//change input.size() to some parameter from parameter file
		for (unsigned int j = 0; j < input.size(); j++)
		{
            int part = (int) input[j].orientation/angle_size;
            double sim = SmoothingFactor;
            if (grid.gridmap[input_loc.x][input_loc.y][part].valid)
            {
                input.at(j).compare(grid.gridmap[input_loc.x][input_loc.y][part], tot_similarity);
            }
            tot_similarity += sim;
		}
		if (tot_similarity > best_similarity)
        {
            double angle = ( i + input.size() / 2 ) *angle_size;
            double w_angle = 2 * M_PI;
            out = fmod(angle, w_angle);
            best_similarity = tot_similarity;
		}
	}
	//return angle (and pos?)
	//add them to the map, if they have better reliability
	for (vector<VisualCompassFeature>::iterator it = input.begin(); it != input.end(); ++it)
	{
		grid.storeFeature(*it);
	}
}
