/**
* @file VisualCompass.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* @author ...
* @author ...
* Definition of class VisualCompass
*/

#ifndef VISUALCOMPASS_H
#define VISUALCOMPASS_H

// others
#include <ModuleFramework/Module.h>
#include <vector>
#include <fstream>
#include <stdio.h>

// tools
#include "Tools/Math/Line.h"
#include "Tools/ColorClasses.h"
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Matrix_nxn.h"
#include "Tools/Math/Matrix_mxn.h"
#include "Tools/Math/Geometry.h"
#include "Tools/ImageProcessing/BresenhamLineScan.h"
#include "Tools/ImageProcessing/ColoredGrid.h"

// infrastructure
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/Image.h"
#include "Representations/Infrastructure/ColorTable64.h"
#include "Representations/Infrastructure/CameraSettings.h"
#include "Representations/Infrastructure/CameraInfo.h"
#include "Representations/Infrastructure/FieldInfo.h"

// perception
#include "Representations/Perception/FieldPercept.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Perception/ArtificialHorizon.h"
#include "Representations/Perception/LinePercept.h"
#include "Representations/Perception/ObjectPercept.h"

// modelling
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Modeling/ColorClassificationModel.h"
#include "Representations/Modeling/AttentionModel.h"

// visual compass stuff
#include "VisualCompassParameters.h"
#include "VisualCompassFeature.h"
#include "VisualGridMapProvider.h"

// motion
#include "Representations/Motion/Request/HeadMotionRequest.h"
#include "Representations/Motion/Request/MotionRequest.h"
#include "Representations/Motion/MotionStatus.h"

//debug requests
#include "Tools/Debug/DebugRequest.h"
#include "Tools/Debug/DebugModify.h"

#include "DebugCommunication/DebugCommandManager.h"



BEGIN_DECLARE_MODULE(VisualCompass)
REQUIRE(Image)
REQUIRE(ColorClassificationModel)
REQUIRE(CurrentCameraSettings)
REQUIRE(CameraMatrix)
REQUIRE(ArtificialHorizon)
REQUIRE(FieldPercept)
REQUIRE(ColoredGrid)
REQUIRE(FieldInfo)
REQUIRE(OpenCVGrayScale)
REQUIRE(FrameInfo)
REQUIRE(MotionStatus)
REQUIRE(RobotPose)

PROVIDE(HeadMotionRequest)
PROVIDE(MotionRequest)
END_DECLARE_MODULE(VisualCompass)

class VisualCompass: private VisualCompassBase
{
public:
    VisualCompass();
    ~VisualCompass();
    void execute();
private:
    vector<Pixel> pixelVector;
    vector<naoth::Image> imageVector;
    void head();
    void motion();

    /*
     *
     */
    bool readModel(vector<naoth::Image> model);
    /*
     *
     */
    bool saveModel();
    /*
     * checks if there is already a model
     */
    bool hasModel();
    /*
     * clears any previous files stored from the compass
     */
    void clearCompass();
    /*
     *returns true if the visual compass has an output to give
     */
    bool isValid();

    /*
     *checks if the horizon leaves us the space to
     *check for features
     */
    bool isValid(Vector2<double> a, Vector2<double> b);

    /*
     *extracts the colors above the horizon
     */
    void colorExtraction();

    /*
     *gets the image and the horizon and scans every 10 pixels
     *a vertical line storing the pixel values into a vector
     */
    vector< vector<Pixel> > verticalScanner();

    /*
     *does the robot to turn 360 degrees, to map the environment
     */
    void mapping(cv::Mat &mappingImages);

    void scanner();


};//end class VisualCompass

#endif // VISUALCOMPASS_H
