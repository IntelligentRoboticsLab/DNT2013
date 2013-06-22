/**
* @file VisualCompassFeature.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
*/

#ifndef VISUALCOMPASSPARAMETERS_H
#define VISUALCOMPASSPARAMETERS_H
#define _USE_MATH_DEFINES

// VisualCompass
#define COMPASS_WIDTH_MIN 0.49
#define COMPASS_WIDTH_MAX 0.80
#define COMPASS_FEATURE_STEP 10
#define COMPASS_FEATURE_START 5.00

// VisualCompassFeature
// ...

// ColorDiscretizer
// ...

// VisualGridMapProvider
#define GridxLength 5
#define GridyLength 3
#define AngleBins 360
#define angle_size 2*M_PI/AngleBins;

// WeightedExperts
#define SmoothingFactor 0.1 //TODO test this value later

#endif // VISUALCOMPASSPARAMETERS_H
