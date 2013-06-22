#include "VisualCompassFeature.h"

VisualCompassFeature::VisualCompassFeature()
{
}

VisualCompassFeature::~VisualCompassFeature()
{
    //nothing here.
}

void VisualCompassFeature::createFeatureFromScanLine(vector<Pixel> scanLine, VisualCompassFeature &produced_feature)
{
}

void VisualCompassFeature::getCertainty(time_t current_time, double &certainty)
{
	certainty = this->measurement_certainty * exp(difftime(this->time, current_time));
}

void VisualCompassFeature::compare(VisualCompassFeature vcf, double &similarity_measure)
{
    similarity_measure = 1.0; 
	//M_PI is the maximum distance possible between 2 angles
	similarity_measure *= abs(this->orientation - vcf.orientation)/M_PI;
	//TODO factor in the certainties
	//TODO normalise distance?
	similarity_measure *= sqrt(pow(this->source_position.x - vcf.source_position.x, 2) + pow(this->source_position.y - vcf.source_position.y, 2));
}
