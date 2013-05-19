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
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Tools/Math/Line.h"
#include "Tools/ColorClasses.h"
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Matrix_nxn.h"
#include "Tools/Math/Matrix_mxn.h"
#include "Tools/Math/Geometry.h"

#include "Tools/ImageProcessing/ImageDrawings.h"
#include "Tools/ImageProcessing/BresenhamLineScan.h"
#include "Tools/ImageProcessing/LineDetectorDNTConstParameters.h"
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
  struct scan_point
  {
      int id;
      Vector2<int> position;
      Vector2<int> position_start;
      Vector2<int> position_end;
      double weight;
      int thickness;
      double distance;
      bool valid;
  };

  struct line_candidate
  {
      vector< scan_point > scan_points;
      scan_point* start;
      scan_point* end;
  };

  vector<LinePercept::LineSegmentImage> lineSegments;

  void determineScanResolution(FieldPercept::FieldPoly fieldPoly, int &scanResolution);

  void scanLinesHorizontal(FieldPercept::FieldPoly fieldPoly, vector< scan_point >& linePoints, int scanResolution, int &point_id);

  void scanLinesVertical(FieldPercept::FieldPoly fieldPoly, vector< scan_point >& linePoints, int scanResolution, int& point_id);

  void candidate_points(vector< scan_point > scan_points, scan_point start, scan_point previous, vector< scan_point > lineTemp, vector<scan_point> &candidates);

  void line_extraction(vector< scan_point > scan_points, vector<vector<scan_point> > &extracted_lines);

  void best_candidate(vector<scan_point> candidates, vector<scan_point> line,  scan_point start, scan_point previous, scan_point &best, double &min_error);

  void compute_white_ratio(scan_point point1, scan_point point2, double &ratio);

  void line_error(vector<scan_point> line, scan_point start, scan_point best_candidate, double &error);

  void delete_point(scan_point element, vector<scan_point> &points);

  void store_line(vector< vector<scan_point> > &lines, vector<scan_point> line);

  const ColorClassificationModel& getColorTable64() const
  {
    return getColorClassificationModel();
  }

};//end class LineDetectorDNT

#endif // __LineDetectorDNT_H_
