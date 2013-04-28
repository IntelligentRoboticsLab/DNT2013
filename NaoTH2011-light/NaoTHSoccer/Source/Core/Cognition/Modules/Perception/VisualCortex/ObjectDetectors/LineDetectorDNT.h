/**
* @file LineDetectorDNT.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* Definition of class LineDetectorDNT
*/

#ifndef _LineDetectorDNT_H_
#define _LineDetectorDNT_H_

#include <ModuleFramework/Module.h>

#include <vector>
#include "Tools/Debug/Stopwatch.h"

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
  // item candidate_point represents points to be connected with the producing line,
  // distance and coordinate.
  struct candidate_point
  {
      Point pnt;
      double distance;
  };

  vector<LinePercept::LineSegmentImage> lineSegments;

  void scanLinesHorizontal(vector< Vector2<int> >& linePoints, int scanResolution, int scanStep, double qualRatio);

  void scanLinesVertical(vector< Vector2<int> >& linePoints, int scanResolution, int scanStep, double qualRatio);

  void find_candidate_points(vector< Vector2<int> > points, Vector2<int> start, Vector2<int> previous, vector< Vector2<int> > line, vector<candidate_point> &candidates);

  void line_extraction(vector< Vector2<int> > points, vector<Vector4d> &produced_lines);

  const ColorClassificationModel& getColorTable64() const
  {
    return getColorClassificationModel();
  }

};//end class LineDetectorDNT

#endif // __LineDetectorDNT_H_
