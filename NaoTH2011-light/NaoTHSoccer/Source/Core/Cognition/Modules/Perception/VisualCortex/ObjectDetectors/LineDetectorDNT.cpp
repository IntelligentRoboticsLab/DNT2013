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
    GT_TRACE("executing LineDetectorDNT~Scanner");
    STOPWATCH_START("LineDetectorDNT~Scanner");
    scanLinesHorizontal(scanPoints, 15, 2, 0.60, point_id);
    scanLinesVertical(scanPoints, 15, 2, 0.60, point_id);
    STOPWATCH_STOP("LineDetectorDNT~Scanner");

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_points",
                  for(int i = 0; i < (int) scanPoints.size(); i ++){
        CIRCLE_PX(ColorClasses::yellow, (int) scanPoints[i].position.x , (int) scanPoints[i].position.y, (int) 1);
        LINE_PX(ColorClasses::red, (int) scanPoints[i].position_start.x, (int) scanPoints[i].position_start.y, (int) scanPoints[i].position_end.x, (int) scanPoints[i].position_end.y);
    }
    );

    vector<cv::Vec4i> lines;
    GT_TRACE("executing LineDetectorDNT~Extraction");
    STOPWATCH_START("LineDetectorDNT~Extraction");
    line_extraction(scanPoints, lines);
    STOPWATCH_STOP("LineDetectorDNT~Extraction");



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

void LineDetectorDNT::compute_white_ratio(scan_point point1, scan_point point2, double &ratio)
{
    int     white_counter = 0,
            all_counter = 0,
            x0 = point1.position.x,
            y0 = point1.position.y,
            x1 = point2.position.x,
            y1 = point2.position.y,
            dx = abs(x1-x0),
            dy = abs(y1-y0),
            err,
            sx = 0,
            sy = 0;

    if (x0 < x1)
    {
        sx = 1;
    }
    else
    {
        sx = -1;
    }
    if (y0 < y1)
    {
        sy = 1;
    }
    else
    {
        sy = -1;
    }
    err = dx-dy;
    while(1)
    {
        all_counter ++;
        Pixel pixel = getImage().get(x0, y0);
        ColorClasses::Color thisPixelColor = getColorTable64().getColorClass(pixel);
        if(thisPixelColor == ColorClasses::white)
        {
            white_counter ++;
        }
        else if(thisPixelColor == ColorClasses::green)
        {
            white_counter --;
        }
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        int e2 = 2*err;
        if (e2 > -dy)
        {
            err = err - dy;
            x0 = x0 + sx;
        }
        if(e2 <  dx)
        {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
    ratio = (double) white_counter/all_counter;
    return;
}

void LineDetectorDNT::best_candidate(vector<scan_point> candidates, vector<scan_point> line,  scan_point start, scan_point previous, scan_point &best, double &min_error)
{
    min_error = DBL_MAX;
    double white;
    double distance;
    double temp_error;
    scan_point temp;

    for(unsigned int i = 0; i < candidates.size(); i++)
    {
        temp = candidates[i];
        temp_error = 0;
        distance = previous.position.dis(temp.position);
        compute_white_ratio(previous, temp, white);
        temp_error = (1.01 - white) * distance;
        if(temp_error < min_error)
        {
            best = temp;
            min_error = temp_error;
        }
    }
    return;
}

void LineDetectorDNT::line_error(vector<scan_point> line, scan_point start, scan_point best_candidate, double &error)
{
    error = 0.00;
    Math::LineSegment temp_line = Math::LineSegment(start.position, best_candidate.position);
    for(unsigned int i = 0; i < line.size(); i++)
    {
        error += pow(temp_line.minDistance(line[i].position),3);
    }
    return;
}

void LineDetectorDNT::delete_point(scan_point element, vector<scan_point> &points)
{
    for(unsigned int i=0; i < points.size(); i++)
    {
        if(element.id == points[i].id)
        {
            points.erase(points.begin() + i);
            break;
        }
    }
    return;
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
        scan_points.pop_back();
        do
        {
            lineTemp.push_back(previous);
            vector<scan_point> candidates;
            candidate_points(scan_points, start, previous, lineTemp, candidates);
            scan_point best;
            double error;
            best_candidate(candidates, lineTemp, start, previous, best, error);
            candidates.clear();
            if(error > 3.00)
            {
                end = true;
            }
            else
            {
                if(lineTemp.size() >= 4)
                {
                    double sum_error;
                    line_error(lineTemp, start, best, sum_error);
                    if(sum_error < 5)
                    {
                        previous = best;
                    }
                    else
                    {
                        if(lineTemp.size() <= 4 && lineTemp.size() != 0)
                        {
                            for(unsigned int i=0; i<lineTemp.size(); i++)
                            {
                                scan_points.push_back(lineTemp[i]);
                            }
                            lineTemp.clear();
                        }
                        end = true;
                    }
                }
                else
                {
                    lineTemp.push_back(best);
                    previous = best;
                }
            }
            delete_point(previous, scan_points);
        }
        while(!end);
    }
}

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
