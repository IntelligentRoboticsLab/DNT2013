/**
* @file VisualCompassFeature.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* @author ...
* @author ...
* Definition of class VisualCompassFeature
*/

#ifndef VISUALCOMPASSFEATURE_H
#define VISUALCOMPASSFEATURE_H

#include <ModuleFramework/Module.h>
#include <vector>
#include "Tools/Math/Line.h"
#include "Tools/ColorClasses.h"
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Matrix_nxn.h"
#include "Tools/Math/Matrix_mxn.h"
#include "Tools/Math/Geometry.h"
#include "Tools/ImageProcessing/ColoredGrid.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/Image.h"
#include "Representations/Infrastructure/ColorTable64.h"
#include "Representations/Infrastructure/CameraSettings.h"
#include "Representations/Infrastructure/FieldInfo.h"
#include "Representations/Perception/FieldPercept.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Perception/ArtificialHorizon.h"
#include "Representations/Perception/LinePercept.h"
#include "Representations/Perception/ObjectPercept.h"
#include "Representations/Modeling/ColorClassificationModel.h"
#include "Cognition/Modules/Infrastructure/OpenCV/OpenCVImageProvider.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualCompassParameters.h"
#include "Cognition/Modules/Perception/VisualCortex/ColorDiscretizer.h"
#include <time.h> 

class VisualCompassFeature
{
public:
    VisualCompassFeature();
    ~VisualCompassFeature();
    /*
     *attributes for these feature type elements
     */
    unsigned int table[NUM_OF_COLORS][NUM_OF_COLORS];
    int length;
    double orientation;
    time_t time;
    Vector2<double> source_position;
	double measurement_certainty;
    bool valid;

    /*
     *gets two features and compares them
     */
    void compare(VisualCompassFeature vcf, double &similarity_measure);

    /*
     *gets a the pixels for a line in the image and creates a
     *feature
     */
    void createFeatureFromScanLine(vector<Pixel> scanLine, const ColorDiscretizer &ClusteringProvider);

	void getCertainty(time_t current_time, double &certainty);
private:
};

#endif // VISUALCOMPASSFEATURE_H
