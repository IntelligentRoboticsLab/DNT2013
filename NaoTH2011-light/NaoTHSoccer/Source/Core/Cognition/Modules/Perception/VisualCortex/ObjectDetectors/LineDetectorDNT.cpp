#include "LineDetectorDNT.h"
#include "Tools/ImageProcessing/LineScanner.h"

LineDetectorDNT::LineDetectorDNT()
{
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:scan_points", "mark the points scanned as line candidates on the image", false);
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:extracted_lines", "mark the extracted lines on the image", false);
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:scan_area", "mark the area scanned", false);
}

void LineDetectorDNT::execute()
{
    lineSegments.clear();
    vector< scan_point > scanPoints;
    FieldPercept::FieldPoly fieldPoly;
    fieldPoly = getFieldPercept().getPoly();
    int scanResolution = 20;
    if(fieldPoly.getArea() > 484591.0)
        scanResolution += 5;

    int point_id = -1;
    GT_TRACE("executing LineDetectorDNT~Scanner");
    STOPWATCH_START("LineDetectorDNT~Scanner");
    scanLinesHorizontal(fieldPoly, scanPoints, scanResolution, 2, 0.60, point_id);
    scanLinesVertical(fieldPoly, scanPoints, scanResolution, 2, 0.60, point_id);
    STOPWATCH_STOP("LineDetectorDNT~Scanner");

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_points",
                  for(int i = 0; i < (int) scanPoints.size(); i ++){
        CIRCLE_PX(ColorClasses::blue, (int) scanPoints[i].position.x , (int) scanPoints[i].position.y, (int) 1);
        LINE_PX(ColorClasses::green, (int) scanPoints[i].position_start.x, (int) scanPoints[i].position_start.y, (int) scanPoints[i].position_end.x, (int) scanPoints[i].position_end.y);
    }
    );


    vector< vector< scan_point > > extracted_lines;
    GT_TRACE("executing LineDetectorDNT~Extraction");
    STOPWATCH_START("LineDetectorDNT~Extraction");
    line_extraction(scanPoints, extracted_lines);
    STOPWATCH_STOP("LineDetectorDNT~Extraction");

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:extracted_lines",
                  for (unsigned int i = 0; i < extracted_lines.size(); ++i) {
        LINE_PX(ColorClasses::red, (int) extracted_lines[i][0].position.x, (int) extracted_lines[i][0].position.y,
                (int) extracted_lines[i][extracted_lines[i].size()-1].position.x, (int) extracted_lines[i][extracted_lines[i].size()-1].position.y);
    });

}//end execute

void LineDetectorDNT::scanLinesHorizontal(FieldPercept::FieldPoly fieldPoly, vector< scan_point >& linePoints, int scanResolution, int scanStep, double qualRatio, int& point_id)
{
    Pixel pixel;
    ColorClasses::Color thisPixelColor;
    for(int i = 0; i < (int) getImage().height(); i += scanResolution)
    {
        vector< Vector2<double> > intersection_points;
        Math::LineSegment al = Math::LineSegment(Vector2<int>(0,i), Vector2<int>(getImage().width(), i));
        for(int var = 1; var < fieldPoly.length; var++)
        {
            Math::LineSegment temp = Math::LineSegment(fieldPoly[var-1], fieldPoly[var]);
            if(temp.intersect(al))
            {
                double a = temp.intersection(al);
                intersection_points.push_back(temp.point(a));
            }
            if(intersection_points.size() >= 2)
            {
                for(unsigned int var2 = 1; var2 < intersection_points.size(); var2++)
                {
                    if(fieldPoly.isInside(Vector2<int>(floor((intersection_points[var2-1].x + intersection_points[var2].x) / 2),
                                                       floor((intersection_points[var2-1].y + intersection_points[var2].y) / 2))))
                    {
                        DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_area",
                                      LINE_PX(ColorClasses::red, (int) intersection_points[var2-1].x, (int) i, (int) intersection_points[var2].x, (int) i);
                        );
                        int start = floor(min(intersection_points[var2-1].x, intersection_points[var2].x));
                        int stop = floor(max(intersection_points[var2-1].x, intersection_points[var2].x));
                        int step = 0;
                        for(int j = start; j < stop; j += scanStep)
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
                                    double whiteRatio = 0.00;
                                    int numOfPixels = 1, numOfWhite = 0;
                                    if(thisPixelColor == ColorClasses::white)
                                        numOfWhite++;
                                    do{
                                        pixel = getImage().get(j++,i);
                                        thisPixelColor = getColorTable64().getColorClass(pixel);
                                        if(thisPixelColor == ColorClasses::white)
                                            numOfWhite ++;
                                        numOfPixels ++;
                                    }
                                    while(thisPixelColor != ColorClasses::green && j < stop);
                                    if(j < stop)
                                    {
                                        whiteRatio = (double) numOfWhite / (double) numOfPixels;
                                        if(whiteRatio > qualRatio)
                                        {
                                            if(getFieldPercept().getLargestValidPoly(getArtificialHorizon()).isInside(Vector2d(j - ceil((numOfPixels)/2), i)))
                                            {
                                                scan_point temp;
                                                temp.id = point_id++;
                                                temp.position = Vector2<int>(j - 1 - floor((numOfPixels)/2), i);
                                                temp.position_start = Vector2<int>(j - 1 - numOfPixels, i);
                                                temp.position_end = Vector2<int>(j - 1, i);
                                                temp.weight = whiteRatio;
                                                temp.thickness = numOfPixels;
                                                temp.distance = 0.0f;
                                                temp.valid = true;
                                                linePoints.push_back(temp);
                                            }
                                        }
                                        step = 0;
                                    }
                                }
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void LineDetectorDNT::scanLinesVertical(FieldPercept::FieldPoly fieldPoly, vector< scan_point > &linePoints, int scanResolution, int scanStep, double qualRatio, int& point_id)
{
    Pixel pixel;
    ColorClasses::Color thisPixelColor;
    for(int i = 0; i < (int) getImage().width(); i += scanResolution)
    {
        vector< Vector2<double> > intersection_points;
        Math::LineSegment al = Math::LineSegment(Vector2<int>(i,0), Vector2<int>(i,getImage().height()));
        for(int var = 1; var < fieldPoly.length; var++)
        {
            Math::LineSegment temp = Math::LineSegment(fieldPoly[var-1], fieldPoly[var]);
            if(temp.intersect(al))
            {
                double a = temp.intersection(al);
                intersection_points.push_back(temp.point(a));
            }
            if(intersection_points.size() >= 2)
            {
                for(unsigned int var2 = 1; var2 < intersection_points.size(); var2++)
                {
                    if(fieldPoly.isInside(Vector2<int>(floor((intersection_points[var2-1].x + intersection_points[var2].x) / 2),
                                                       floor((intersection_points[var2-1].y + intersection_points[var2].y) / 2))))
                    {
                        DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_area",
                                      LINE_PX(ColorClasses::red, (int) i, (int) intersection_points[var2-1].y, (int) i, (int) intersection_points[var2].y);
                        );
                        int start = floor(min(intersection_points[var2-1].y, intersection_points[var2].y));
                        int stop = floor(max(intersection_points[var2-1].y, intersection_points[var2].y));
                        int step = 0;
                        for(int j = start; j < stop; j += scanStep)
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
                                    double whiteRatio = 0.00;
                                    int numOfPixels = 1, numOfWhite = 0;
                                    if(thisPixelColor == ColorClasses::white)
                                        numOfWhite++;
                                    do{
                                        pixel = getImage().get(i,j++);
                                        thisPixelColor = getColorTable64().getColorClass(pixel);
                                        if(thisPixelColor == ColorClasses::white)
                                            numOfWhite ++;
                                        numOfPixels ++;
                                    }
                                    while(thisPixelColor != ColorClasses::green && j < stop);
                                    if(j < stop)
                                    {
                                        whiteRatio = (double) numOfWhite / (double) numOfPixels;
                                        if(whiteRatio > qualRatio)
                                        {
                                            if(getFieldPercept().getLargestValidPoly(getArtificialHorizon()).isInside(Vector2d(i,j - floor((numOfPixels)/2))))
                                            {
                                                scan_point temp;
                                                temp.id = point_id++;
                                                temp.position = Vector2<int>(i, j - 1 - floor((numOfPixels)/2));
                                                temp.position_start = Vector2<int>(i, j - 1 - numOfPixels);
                                                temp.position_end = Vector2<int>(i, j - 1);
                                                temp.weight = whiteRatio;
                                                temp.thickness = numOfPixels;
                                                temp.distance = 0.0f;
                                                temp.valid = true;
                                                linePoints.push_back(temp);
                                            }
                                        }
                                        step = 1;
                                    }
                                }
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
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
            double temp_sim_value = previous.position.dis(scan_points[var].position) + abs(previous.thickness - scan_points[var].thickness);
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
            ratio = 0.00;
            return;
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

void LineDetectorDNT::store_line(vector< vector<scan_point> > &lines, vector<scan_point> line)
{
    return;
}

void LineDetectorDNT::line_extraction(vector< scan_point > scan_points, vector< vector< scan_point > > &extracted_lines)
{
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
                    if(sum_error < 10)
                    {
                        lineTemp.push_back(best);
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
        extracted_lines.push_back(lineTemp);
        lineTemp.clear();
    }
}
