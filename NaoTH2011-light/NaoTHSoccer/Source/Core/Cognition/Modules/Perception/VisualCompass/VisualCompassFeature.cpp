#include "VisualCompassFeature.h"

VisualCompassFeature::VisualCompassFeature()
{
    for(unsigned int index = 0; index < COMPASS_FEATURE_NUMBER; index++)
    {
        for(unsigned int i = 0; i < NUM_OF_COLORS; i++)
        {
            for(unsigned int j = 0; j < NUM_OF_COLORS; j++)
            {
                this->featureTable2D[index][i][j] = 123456.78;
            }
        }
    }
}

VisualCompassFeature::~VisualCompassFeature()
{
    //nothing here.
}

void VisualCompassFeature::createFeatureFromScanLine(vector< vector<Pixel> > scanLine, ColorDiscretizer ClusteringProvider)
{
    if(scanLine.size() == 0) return;
    for(unsigned int stripe = 0; stripe < scanLine.size(); stripe++)
    {
        vector<int> labels;
        ClusteringProvider.discretize(scanLine.at(stripe), labels);
        for(unsigned int i = 1; i < labels.size(); i++)
        {
            this->featureTable2D[stripe][labels.at(i-1)][labels.at(i)] += (double) 1 / (double) labels.size();
        } 
    }
    return;
}

void VisualCompassFeature::getCertainty(time_t current_time, double &certainty)
{
    certainty = this->measurement_certainty * exp(difftime(this->time, current_time));
}

void VisualCompassFeature::compare(VisualCompassFeature vcf, long double &similarity_measure)
{
    similarity_measure = 0.00;
    for(unsigned int index = 0; index < COMPASS_FEATURE_NUMBER; index++)
    {
        for(unsigned int i = 0; i < NUM_OF_COLORS; i++)
        {
            for(unsigned int j = 0; j < NUM_OF_COLORS; j++)
            {
                similarity_measure += abs(this->featureTable2D[index][i][j] - vcf.featureTable2D[index][i][j]);
            }
        }
    }
    return;
}
