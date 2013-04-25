#ifndef _LineDetectorDNT_H_
#define _LineDetectorDNT_H_

#include <ModuleFramework/Module.h>

#include <vector>

#include "Tools/Math/Line.h"
#include "Tools/ColorClasses.h"
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Matrix_nxn.h"
#include "Tools/Math/Matrix_mxn.h"
#include "Tools/Math/Geometry.h"

#include "Tools/ImageProcessing/ImageDrawings.h"
#include "Tools/ImageProcessing/BresenhamLineScan.h"
#include "Tools/ImageProcessing/LineDetectorConstParameters.h"
#include "Tools/ImageProcessing/ClusteredLine.h"
#include "Tools/ImageProcessing/Edgel.h"
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
#include "Representations/Perception/ScanLineEdgelPercept.h"
#include "Representations/Modeling/ColorClassificationModel.h"

BEGIN_DECLARE_MODULE(LineDetectorDNT)
  REQUIRE(Image)
  REQUIRE(ColorClassificationModel)
  REQUIRE(CurrentCameraSettings)
  REQUIRE(CameraMatrix)
  REQUIRE(ArtificialHorizon)
  REQUIRE(FieldPercept)
  REQUIRE(ColoredGrid)
  REQUIRE(FieldInfo)
  REQUIRE(ScanLineEdgelPercept)
  REQUIRE(FrameInfo)

  PROVIDE(LinePercept)
  PROVIDE(ObjectPercept)
END_DECLARE_MODULE(LineDetectorDNT)


class LineDetectorDNT: private LineDetectorDNTBase
{
public:
  LineDetectorDNT();
  ~LineDetectorDNT(){};

  void execute();

private:
  vector<LinePercept::LineSegmentImage> lineSegments;

  void scanLinesHorizontal(vector<Vector2d>& linePoints);

  void scanLinesVertical(vector<Vector2d>& linePoints);

  const ColorClassificationModel& getColorTable64() const
  {
    return getColorClassificationModel();
  }

};//end class LineDetectorDNT

#endif // __LineDetectorDNT_H_
