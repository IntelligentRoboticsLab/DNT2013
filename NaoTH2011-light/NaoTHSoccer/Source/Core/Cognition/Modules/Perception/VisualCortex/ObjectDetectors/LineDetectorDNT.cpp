#include "LineDetectorDNT.h"
#include "Tools/ImageProcessing/LineScanner.h"

LineDetectorDNT::LineDetectorDNT()
{
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:scan_points", "mark the points scanned as line candidates on the image", false);
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:extracted_lines", "mark the extracted lines on the image", false);
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:scan_area", "mark the area scanned", false);
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:clustered lines", "mark the clustered lines", false);
}

void LineDetectorDNT::execute()
{
    int point_id = -1;
    int scanResolution;
    lineSegments.clear();
    vector< scan_point > scanPoints;
    FieldPercept::FieldPoly fieldPoly;
    vector< Math::LineSegment > extracted_lines;

    fieldPoly = getFieldPercept().getPoly();
    determineScanResolution(fieldPoly, scanResolution);

    GT_TRACE("executing LineDetectorDNT~Scanner");
    STOPWATCH_START("LineDetectorDNT~Scanner");
    scanLinesHorizontal(fieldPoly, scanPoints, scanResolution, point_id);
    scanLinesVertical(fieldPoly, scanPoints, scanResolution, point_id);
    STOPWATCH_STOP("LineDetectorDNT~Scanner");

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_points",
                  for(int i = 0; i < (int) scanPoints.size(); i ++){
        CIRCLE_PX(ColorClasses::blue, (int) scanPoints[i].position.x , (int) scanPoints[i].position.y, (int) 1);
        LINE_PX(ColorClasses::green, (int) scanPoints[i].position_begin.x, (int) scanPoints[i].position_begin.y, (int) scanPoints[i].position_end.x, (int) scanPoints[i].position_end.y);
    }
    );

    GT_TRACE("executing LineDetectorDNT~Extraction");
    STOPWATCH_START("LineDetectorDNT~Extraction");
    line_extraction(scanPoints, extracted_lines);
    STOPWATCH_STOP("LineDetectorDNT~Extraction");


    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:extracted_lines",
    for(unsigned int i = 0; i < extracted_lines.size(); i ++)
    {
        LINE_PX(ColorClasses::red, (int) extracted_lines[i].begin().x,
                (int) extracted_lines[i].begin().y,
                (int) extracted_lines[i].end().x,
                (int) extracted_lines[i].end().y);
    }
    );

}//end execute

void LineDetectorDNT::determineScanResolution(FieldPercept::FieldPoly fieldPoly, int &scanResolution)
{
    double  ratio = 1.0 - min(1.0, fieldPoly.getArea() / MAX_AREA);
    scanResolution = MAX_SCAN_RESOLUTION - floor((MAX_SCAN_RESOLUTION - MIN_SCAN_RESOLUTION) * ratio);
}

void LineDetectorDNT::scanLinesHorizontal(FieldPercept::FieldPoly fieldPoly, vector< scan_point >& linePoints, int scanResolution, int& point_id)
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
                        int start = floor(min(intersection_points[var2-1].x, intersection_points[var2].x));
                        int stop = floor(max(intersection_points[var2-1].x, intersection_points[var2].x));
                        int step = 0;
                        for(int j = start; j < stop; j += SCAN_STEP)
                        {
                            if(getBodyContour().isOccupied(Vector2<int>(j, i)))
                            {
                                step = 0;
                                continue;
                            }
                            DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_area",
                                          POINT_PX(ColorClasses::gray, (int) j , (int) i);
                                    );
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
                                    int numOfPixels = 0, numOfWhite = 0;

                                    // previous possibly missing pixel
                                    for(int temp= 0; temp < SCAN_STEP; temp ++){
                                        pixel = getImage().get(j - temp,i);
                                        thisPixelColor = getColorTable64().getColorClass(pixel);
                                        if(thisPixelColor == ColorClasses::green)
                                            break;
                                        else if(thisPixelColor == ColorClasses::white)
                                            numOfWhite++;
                                        numOfPixels++;
                                    }
                                    bool valid = true;
                                    do{
                                        if(getBodyContour().isOccupied(Vector2<int>(j, i)))
                                        {
                                            step = 0;
                                            valid = false;
                                            break;
                                        }
                                        else
                                        {
                                            DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_area",
                                                          POINT_PX(ColorClasses::gray, (int) j , (int) i);

                                                    );
                                        }
                                        pixel = getImage().get(j++,i);
                                        thisPixelColor = getColorTable64().getColorClass(pixel);
                                        if(thisPixelColor == ColorClasses::white)
                                            numOfWhite ++;
                                        numOfPixels ++;
                                    }
                                    while(thisPixelColor != ColorClasses::green && j < stop);
                                    if(j < stop && valid)
                                    {
                                        whiteRatio = (double) numOfWhite / (double) numOfPixels;
                                        if(whiteRatio > QUAL_WHITE_RATIO && numOfPixels <= MAX_LINE_THICKNESS)
                                        {
                                            scan_point temp;
                                            temp.id = point_id++;
                                            temp.position = Vector2<int>(j - 1 - floor((numOfPixels)/2), i);
                                            temp.position_begin = Vector2<int>(j - 1 - numOfPixels, i);
                                            temp.position_end = Vector2<int>(j - 1, i);
                                            temp.weight = whiteRatio;
                                            temp.thickness = numOfPixels;
                                            temp.distance = 0.0f;
                                            temp.valid = true;
                                            temp.type = 1;
                                            linePoints.push_back(temp);
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
} // end scanLinesHorizontal

void LineDetectorDNT::scanLinesVertical(FieldPercept::FieldPoly fieldPoly, vector< scan_point > &linePoints, int scanResolution, int& point_id)
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
                        int start = floor(min(intersection_points[var2-1].y, intersection_points[var2].y));
                        int stop = floor(max(intersection_points[var2-1].y, intersection_points[var2].y));
                        int step = 0;
                        for(int j = start; j < stop; j += SCAN_STEP)
                        {
                            if(getBodyContour().isOccupied(Vector2<int>(i, j)))
                            {
                                step = 0;
                                continue;
                            }
                            DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_area",
                                          POINT_PX(ColorClasses::gray, (int) i , (int) j);
                                    );
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
                                    int numOfPixels = 0, numOfWhite = 0;

                                    // previous possibly missing pixel
                                    for(int temp= 0; temp < SCAN_STEP; temp ++){
                                        pixel = getImage().get(i, j - temp);
                                        thisPixelColor = getColorTable64().getColorClass(pixel);
                                        if(thisPixelColor == ColorClasses::green)
                                            break;
                                        else if(thisPixelColor == ColorClasses::white)
                                            numOfWhite++;
                                        numOfPixels++;
                                    }
                                    bool valid = true;
                                    do{
                                        if(getBodyContour().isOccupied(Vector2<int>(i, j)))
                                        {
                                            step = 0;
                                            valid = false;
                                            break;
                                        }
                                        else
                                        {
                                            DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:scan_area",
                                                          POINT_PX(ColorClasses::gray, (int) i , (int) j);

                                                    );
                                        }
                                        pixel = getImage().get(i,j++);
                                        thisPixelColor = getColorTable64().getColorClass(pixel);
                                        if(thisPixelColor == ColorClasses::white)
                                            numOfWhite ++;
                                        numOfPixels ++;
                                    }
                                    while(thisPixelColor != ColorClasses::green && j < stop);
                                    if(j < stop && valid)
                                    {
                                        whiteRatio = (double) numOfWhite / (double) numOfPixels;
                                        if(whiteRatio > QUAL_WHITE_RATIO && numOfPixels <= MAX_LINE_THICKNESS)
                                        {
                                            scan_point temp;
                                            temp.id = point_id++;
                                            temp.position = Vector2<int>(i, j - 1 - floor((numOfPixels)/2));
                                            temp.position_begin = Vector2<int>(i, j - 1 - numOfPixels);
                                            temp.position_end = Vector2<int>(i, j - 1);
                                            temp.weight = whiteRatio;
                                            temp.thickness = numOfPixels;
                                            temp.distance = 0.0f;
                                            temp.valid = true;
                                            temp.type = 2;
                                            linePoints.push_back(temp);
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
} // end scanLinesVertical

void LineDetectorDNT::white_ratio(Vector2<int> point1, Vector2<int> point2, double &ratio)
{
    int     white_counter = 0,
            all_counter = 0,
            x0 = point1.x,
            y0 = point1.y,
            x1 = point2.x,
            y1 = point2.y,
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

void LineDetectorDNT::line_error(line_candidate line, scan_point start, scan_point point, double &error)
{
    error = 0.00;
    Math::LineSegment temp_line = Math::LineSegment(start.position, point.position);
    for(unsigned int i = 0; i < line.scan_points.size(); i++)
    {
        error += pow(temp_line.minDistance(line.scan_points[i].position),3);
    }
    return;
}

double LineDetectorDNT::length(line_candidate line)
{
    return line.begin.position.dis(line.end.position);
}

double LineDetectorDNT::length_start(line_candidate line, Vector2<int> point)
{
    return line.begin.position.dis(point);
}

double LineDetectorDNT::length_end(line_candidate line, Vector2<int> point)
{
    return line.end.position.dis(point);
}

void LineDetectorDNT::find_candidates(vector< scan_point > scan_points, line_candidate line, vector<point_candidate> &candidates)
{
    for (unsigned int var = 0; var < scan_points.size(); var++)
    {
        point_candidate tmp;
        double temp_sim_value = 0.5 * line.begin.position.dis(scan_points[var].position) + abs(line.begin.thickness - scan_points[var].thickness);
        if(line.hasEnd)
        {
            if(length_start(line, scan_points[var].position) < length_end(line, scan_points[var].position))
            {
                temp_sim_value = 0.5 * line.begin.position.dis(scan_points[var].position) + abs(line.begin.thickness - scan_points[var].thickness);
            }
            else
            {
                temp_sim_value = 0.5 * line.end.position.dis(scan_points[var].position) + abs(line.end.thickness - scan_points[var].thickness);
            }
        }

        if(candidates.size() >= BEST_CANDIDATE_BUFFER)
        {
            for(unsigned int j=0; j < candidates.size(); j++)
            {
                for(unsigned int var2=0; var2 < j; var2++)
                {
                    if(candidates[j].score < candidates[var2].score)
                    {
                        tmp = candidates[j];
                        candidates[j] = candidates[var2];
                        candidates[var2] = tmp;
                    }
                }
            }
            if(temp_sim_value < candidates[candidates.size() - 1].score)
            {
                candidates.erase(candidates.begin() + candidates.size() - 1);
                tmp.pos = var;
                tmp.score = temp_sim_value;
                candidates.push_back(tmp);
            }
        }
        else
        {
            tmp.pos = var;
            tmp.score = temp_sim_value;
            candidates.push_back(tmp);
        }
    }
}

bool LineDetectorDNT::connect_single(vector< scan_point > &scan_points, line_candidate &line, vector< point_candidate > &candidates)
{
    scan_point connection;
    double m_error = DBL_MAX;
    double white;
    double error;
    point_candidate index;
    int index_can;
    for(unsigned int i = 0; i < candidates.size(); i++)
    {
        //candidate point
        point_candidate tmp = candidates[i];
        scan_point point = scan_points[tmp.pos];

        //score
        white_ratio(line.begin.position, point.position, white);
        error = (1.01 - white) * tmp.score;

        //take the minimum
        if(error < m_error){
            m_error = error;
            connection = point;
            index = tmp;
        }
    }
    // point connected
    if(m_error < CONN_ERROR_SINGLE)
    {
        line.scan_points.push_back(connection);
        line.end = connection;
        line.hasEnd = true;
        scan_points.erase(scan_points.begin() + index.pos);
        candidates.erase(candidates.begin() + index_can);
        return true;
    }
    return false;
}

bool LineDetectorDNT::connect_complex(vector< scan_point > &scan_points, line_candidate &line, vector< point_candidate > &candidates)
{
    scan_point connection;
    double m_error = DBL_MAX;
    double white;
    double error;
    point_candidate index;
    int index_can;
    bool start = true;
    for(unsigned int i = 0; i < candidates.size(); i++)
    {
        error = 0.00;
        //candidate point
        point_candidate tmp = candidates[i];
        scan_point point = scan_points[tmp.pos];

        double line_start_len = length_start(line, point.position);
        double line_end_len = length_end(line, point.position);
        double line_len = length(line);

        //new line smaller than the previous...
        if(line_len > line_start_len && line_len > line_end_len)
            error += DBL_MAX;

        double l_error;
        bool start_tmp = false;
        //the new point will be connected to the ending point
        if(line_start_len > line_end_len)
        {
            start_tmp = true;
            line_error(line, line.begin, point, l_error);
            white_ratio(line.end.position, point.position, white);
            error += (1.5 - white) * l_error;
        }
        else
        {
            line_error(line, line.end, point, l_error);
            white_ratio(line.begin.position, point.position, white);
            error += (1.5 - white) * l_error;
        }
        //keep the best
        if(error < m_error)
        {
            start = start_tmp;
            m_error = error;
            connection = point;
            index_can = i;
            index = tmp;
        }
    }
    if(m_error < CONN_ERROR_COMPLEX)
    {
        line.scan_points.push_back(connection);
        line.begin = (start) ? line.begin : connection;
        line.end = (!start) ? line.end : connection;
        scan_points.erase(scan_points.begin() + index.pos);
        candidates.erase(candidates.begin() + index_can);
        return true;
    }
    return false;
}

void LineDetectorDNT::explore_candidates(vector< scan_point > &scan_points, line_candidate &line, vector<point_candidate> &candidates, bool &end_b)
{
    end_b = true;
    bool end;
    do
    {
        end = true;
        if(!line.hasEnd)
        {
            if(connect_single(scan_points, line, candidates))
                end = false, end_b = false;
        }
        else
        {
            if(connect_complex(scan_points, line, candidates))
                end = false, end_b = false;
        }
        if(candidates.size() == 0)
            end_b = true; return;

    }
    while(!end);
    return;
}

void LineDetectorDNT::store_line(line_candidate line, vector< Math::LineSegment > &extracted_lines)
{
    if(line.hasEnd)
    {
        Math::LineSegment tmp = Math::LineSegment(line.begin.position, line.end.position);
        extracted_lines.push_back(tmp);
    }
}

void LineDetectorDNT::line_extraction(vector< scan_point > scan_points, vector< Math::LineSegment > &extracted_lines)
{
    while(scan_points.size() != 0)
    {
        bool end = false;
        line_candidate lineTemp;
        lineTemp.scan_points.push_back(scan_points[0]);
        lineTemp.begin = scan_points[0];
        lineTemp.hasEnd = false;
        scan_points.erase(scan_points.begin());
        do
        {
            vector<point_candidate> candidates;
            find_candidates(scan_points, lineTemp, candidates);
            explore_candidates(scan_points, lineTemp, candidates, end);
        }
        while(!end);
        store_line(lineTemp, extracted_lines);

    }
} //end line_extraction
