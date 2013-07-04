#include "VisualCompassFeature.h"

VisualCompassFeature::VisualCompassFeature()
{
}

VisualCompassFeature::~VisualCompassFeature()
{
    //nothing here.
}

void VisualCompassFeature::createFeatureFromScanLine(vector<Pixel> scanLine, ColorDiscretizer& ClusteringProvider)
{
    if(scanLine.size() == 0) return;
    vector<int> labels;
    ClusteringProvider.discretize(scanLine, labels);
    for(unsigned int i = 1; i < labels.size(); i++)
    {
        // here the knn will decide the class of each pixel
        std::cout << labels.at(i) << std::endl;
        this->featureTable2D[labels.at(i-1)][labels.at(i)]++;
    }
    return;
}

void VisualCompassFeature::getCertainty(time_t current_time, double &certainty)
{
	certainty = this->measurement_certainty * exp(difftime(this->time, current_time));
}

void VisualCompassFeature::compare(VisualCompassFeature vcf, double &similarity_measure)
{
    similarity_measure = 0;
    for(unsigned int i = 0; i < NUM_OF_COLORS; i++)
    {
        for(unsigned int j = 0; j < NUM_OF_COLORS; j++)
        {
            similarity_measure += this->featureTable2D[i][j] - vcf.featureTable2D[i][j];
        }
    }
    return;
}
