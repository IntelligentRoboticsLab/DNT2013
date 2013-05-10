#include "LineDetectorDNT.h"
#include "Tools/ImageProcessing/LineScanner.h"

LineDetectorDNT::LineDetectorDNT()
{
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:scan_points", "mark the points scanned as line candidates on the image", false);
}

void LineDetectorDNT::execute()
{
    lineSegments.clear();
    vector< scan_point > scanPoints;
    int point_id = -1;
    scanLinesHorizontal(scanPoints, 15, 2, 0.60, point_id);
    scanLinesVertical(scanPoints, 15, 2, 0.60, point_id);

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_points",
                  for(int i = 0; i < (int) scanPoints.size(); i ++){
        CIRCLE_PX(ColorClasses::yellow, (int) scanPoints[i].position.x , (int) scanPoints[i].position.y, (int) 1);
        LINE_PX(ColorClasses::red, (int) scanPoints[i].position_start.x, (int) scanPoints[i].position_start.y, (int) scanPoints[i].position_end.x, (int) scanPoints[i].position_end.y);
    }
    );

    vector<cv::Vec4i> lines;
    line_extraction(scanPoints, lines);


}//end execute

void LineDetectorDNT::scanLinesHorizontal(vector< scan_point >& linePoints, int scanResolution, int scanStep, double qualRatio, int& point_id)
{
    Pixel pixel;
    ColorClasses::Color thisPixelColor;

    for(int i = 0; i < (int) getImage().height(); i += scanResolution)
    {
        int step = 0;
        for(int j = 0; j < (int) getImage().width(); j += scanStep)
        {
            pixel = getImage().get(j,i);
            thisPixelColor = getColorTable64().getColorClass(pixel);
            switch (step)
            {
            case 0:
                if(thisPixelColor == ColorClasses::green)
                    step = 1;
                break;
            case 1:
                if(thisPixelColor == ColorClasses::white || thisPixelColor == ColorClasses::none)
                {
                    step = 2;
                }
                break;
            case 2:
            {
                double whiteRatio = 0.00;
                int numOfPixels = -1, numOfWhite = 0;
                do{
                    pixel = getImage().get(j++,i);
                    thisPixelColor = getColorTable64().getColorClass(pixel);
                    if(thisPixelColor == ColorClasses::white)
                        numOfWhite ++;
                    numOfPixels ++;
                }
                while(thisPixelColor != ColorClasses::green && j < (int) getImage().width());
                if(j < (int) getImage().width())
                {
                    whiteRatio = (double) numOfWhite / (double) numOfPixels;
                    if(whiteRatio > qualRatio)
                    {
                        if(getFieldPercept().getLargestValidPoly(getArtificialHorizon()).isInside(Vector2d(j - ceil((numOfPixels)/2), i)))
                        {
                            scan_point temp;
                            temp.id = point_id++;
                            temp.position = Vector2<int>(j - ceil((numOfPixels)/2), i);
                            temp.position_start = Vector2<int>(j - numOfPixels, i);
                            temp.position_end = Vector2<int>(j, i);
                            temp.weight = whiteRatio;
                            temp.thickness = numOfPixels;
                            temp.distance = 0.0f;
                            temp.valid = true;
                            linePoints.push_back(temp);
                        }
                    }
                    step = 1;
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

void LineDetectorDNT::scanLinesVertical(vector< scan_point > &linePoints, int scanResolution, int scanStep, double qualRatio, int& point_id)
{
    Pixel pixel;
    ColorClasses::Color thisPixelColor;

    for(int i = 0; i < (int) getImage().width(); i += scanResolution)
    {
        int step = 0;
        for(int j = 0; j < (int) getImage().height(); j += scanStep)
        {
            pixel = getImage().get(i,j);
            thisPixelColor = getColorTable64().getColorClass(pixel);
            switch (step)
            {
            case 0:
                if(thisPixelColor == ColorClasses::green)
                    step = 1;
                break;
            case 1:
                if(thisPixelColor == ColorClasses::white || thisPixelColor == ColorClasses::none)
                {
                    step = 2;
                }
                break;
            case 2:
            {
                double whiteRatio = 0.00;
                int numOfPixels = -1, numOfWhite = 0;
                do{
                    pixel = getImage().get(i,j++);
                    thisPixelColor = getColorTable64().getColorClass(pixel);
                    if(thisPixelColor == ColorClasses::white)
                        numOfWhite ++;
                    numOfPixels ++;
                }
                while(thisPixelColor != ColorClasses::green && j < (int) getImage().height());
                if(j < (int) getImage().height())
                {
                    whiteRatio = (double) numOfWhite / (double) numOfPixels;
                    if(whiteRatio > qualRatio)
                    {
                        if(getFieldPercept().getLargestValidPoly(getArtificialHorizon()).isInside(Vector2d(i,j - floor((numOfPixels)/2))))
                        {
                            scan_point temp;
                            temp.id = point_id++;
                            temp.position = Vector2<int>(i, j - floor((numOfPixels)/2));
                            temp.position_start = Vector2<int>(i, j - numOfPixels);
                            temp.position_end = Vector2<int>(i, j);
                            temp.weight = whiteRatio;
                            temp.thickness = numOfPixels;
                            temp.distance = 0.0f;
                            temp.valid = true;
                            linePoints.push_back(temp);
                        }
                    }
                    step = 1;
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

void LineDetectorDNT::candidate_points(vector< scan_point > scan_points, scan_point start, scan_point previous, vector< scan_point > lineTemp, vector<scan_point> &candidates)
{
    for (unsigned int var = 0; var < scan_points.size(); var++)
    {
        if (scan_points[var].id != previous.id)
        {
            double temp_sim_value = previous.position.dis(scan_points[var].position);
            if(lineTemp.size() >= 3)
            {
                Math::LineSegment temp_line = Math::LineSegment(start.position, previous.position);
                temp_sim_value = temp_sim_value * 0.05 + temp_line.minDistance(scan_points[var].position);
            }
            if(candidates.size() >= 5)
            {
                // bubblesort
                for(unsigned int j=0; j < candidates.size(); j++)
                {
                    for(unsigned int var2=0; var2 < j; var2++)
                    {
                        if(candidates[j].distance < candidates[var2].distance)
                        {
                            scan_point temp = candidates[j];
                            candidates[j] = candidates[var2];
                            candidates[var2] = temp;
                        }
                    }
                }
                if(temp_sim_value < candidates[candidates.size() - 1].distance)
                {
                    candidates.erase(candidates.begin() + candidates.size() - 1);
                    scan_points[var].distance = temp_sim_value;
                    candidates.push_back(scan_points[var]);
                }
            }
            else
            {
                scan_points[var].distance = temp_sim_value;
                candidates.push_back(scan_points[var]);
            }
        }
    }
}

void LineDetectorDNT::line_extraction(vector< scan_point > scan_points, vector<cv::Vec4i> &lines)
{
    vector< vector< scan_point > > extracted_lines;
    while(scan_points.size() != 0)
    {
        bool end = false;
        vector< scan_point > lineTemp;
        scan_point start = scan_points[0];
        scan_point previous = scan_points[0];
        do
        {
            lineTemp.push_back(previous);
            vector<scan_point> candidates;
            candidate_points(scan_points, start, previous, lineTemp, candidates);

        }
        while(!end);
    }
}

//void LineDetectorDNT::find_candidate_points(vector< Vector2<int> > points, Vector2<int> start, Vector2<int> previous, vector< Vector2<int> > line, vector<candidate_point> &candidates)
//{
//    for(int i=0; i < points.size(); i++)
//    {
//        if(!equal_points(points[i],previous))
//        {
//            double temp_sim_value = points_distance(previous, points[i]);
//            if(line.size() >= 3)
//            {
//                temp_sim_value = temp_sim_value * 0.05 + point_line_distance(points[i], Vec4i(start.x, start.y, previous.x, previous.y));
//            }
//            if(candidates.size() == 5)
//            {
//                for(int j=0; j<candidates.size(); j++)
//                {
//                    for(int l=0; l<j; l++)
//                    {
//                        if(candidates[j].distance < candidates[l].distance)
//                        {
//                            point_dis temp=candidates[j];
//                            candidates[j]=candidates[l];
//                            candidates[l]=temp;
//                        }
//                    }
//                }
//                if(temp_sim_value < candidates[candidates.size() - 1].distance)
//                {
//                    candidates.erase(candidates.begin() + candidates.size() - 1);
//                    point_dis temp;
//                    temp.pnt = points[i];
//                    temp.distance = temp_sim_value;
//                    candidates.push_back(temp);
//                }
//            }
//            else
//            {
//                point_dis temp;
//                temp.pnt = points[i];
//                temp.distance = temp_sim_value;
//                candidates.push_back(temp);
//            }
//        }
//    }
//}

//void LineDetectorDNT::line_extraction(vector< Vector2<int> > points, vector<Vector4d> &produced_lines)
//{
//    vector< vector<Point> > lines;
//    while(points.size() != 0)
//    {
//        bool end = false;
//        vector< Vector2<int> > line;
//        Vector2<int> start = points[0];
//        Vector2<int> previous = points[0];
//        do
//        {
//            line.push_back(previous);
//            vector<candidate_point> candidates;
//            find_candidate_points(points, start,  previous, line, candidates);
//            // find best candidate to connect
//            Point best_candidate;
//            double error;
//            find_best_candidate(image, candidates, line, start, previous, best_candidate, error);
//            candidates.clear();
//            if(error > 3)
//            {
//                end = true;
//            }
//            else
//            {
//                if(line.size() >= 4)
//                {
//                    double sum_error;
//                    line_error(line, start, best_candidate, sum_error);
//                    if(sum_error < 5)
//                    {
//                        previous = best_candidate;
//                    }
//                    else
//                    {
//                        if(line.size() <= 4 && line.size() != 0)
//                        {
//                            for(int i=0; i<line.size(); i++)
//                            {
//                                points.push_back(line[i]);
//                            }
//                            line.clear();
//                        }
//                        end = true;
//                    }
//                }
//                else
//                {
//                    line.push_back(best_candidate);
//                    previous = best_candidate;
//                }
//            }
//            delete_point(previous, points);
//        }
//        while(!end);
//        store_line(image, lines, line);
//        line.clear();
//    }
//    //lines construction and export...
//    for(int i = 0; i < lines.size(); i++)
//    {
//        Point point1,point2;
//        double max_distance = 0;
//        for(int j1 = 0; j1 < lines[i].size(); j1++)
//        {
//            for(int j2 = 0; j2 < lines[i].size(); j2++)
//            {
//                if(j1 != j2)
//                {
//                    double temp = points_distance(lines[i][j1], lines[i][j2]);
//                    if(temp > max_distance)
//                    {
//                        point1 = lines[i][j1];
//                        point2 = lines[i][j2];
//                        max_distance = temp;
//                    }
//                }
//            }
//        }
//        produced_lines.push_back(Vec4i(point1.x, point1.y, point2.x, point2.y));
//    }
//}
