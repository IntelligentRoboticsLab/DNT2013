#include "LineDetectorDNT.h"
#include "Tools/ImageProcessing/LineScanner.h"

LineDetectorDNT::LineDetectorDNT()
{
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:dntlines", "mark the line candidates on the image", false);
}

void LineDetectorDNT::execute()
{
    lineSegments.clear();
    vector< Vector2<int> > scanPoints;

    scanLinesHorizontal(scanPoints, 12, 2, 0.60);
    scanLinesVertical(scanPoints, 12, 2, 0.60);

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:dntlines",
    for(int i = 0; i < (int) scanPoints.size(); i ++){
        CIRCLE_PX(ColorClasses::orange, (int) scanPoints[i].x -2 , (int) scanPoints[i].y -2, (int) 2);
    }
    );


}//end execute

void LineDetectorDNT::scanLinesHorizontal(vector< Vector2<int> >& linePoints, int scanResolution, int scanStep, double qualRatio)
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

                            linePoints.push_back(Vector2<int>(j - ceil((numOfPixels)/2), i));
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

void LineDetectorDNT::scanLinesVertical(vector< Vector2<int> > &linePoints, int scanResolution, int scanStep, double qualRatio)
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
                            linePoints.push_back(Vector2<int>(i,j - floor((numOfPixels)/2)));
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

void LineDetectorDNT::find_candidate_points(vector< Vector2<int> > points, Vector2<int> start, Vector2<int> previous, vector< Vector2<int> > line, vector<candidate_point> &candidates)
{
    for(int i=0; i < points.size(); i++)
    {
        if(!equal_points(points[i],previous))
        {
            double temp_sim_value = points_distance(previous, points[i]);
            if(line.size() >= 3)
            {
                temp_sim_value = temp_sim_value * 0.05 + point_line_distance(points[i], Vec4i(start.x, start.y, previous.x, previous.y));
            }
            if(candidates.size() == 5)
            {
                for(int j=0; j<candidates.size(); j++)
                {
                    for(int l=0; l<j; l++)
                    {
                        if(candidates[j].distance < candidates[l].distance)
                        {
                            point_dis temp=candidates[j];
                            candidates[j]=candidates[l];
                            candidates[l]=temp;
                        }
                    }
                }
                if(temp_sim_value < candidates[candidates.size() - 1].distance)
                {
                    candidates.erase(candidates.begin() + candidates.size() - 1);
                    point_dis temp;
                    temp.pnt = points[i];
                    temp.distance = temp_sim_value;
                    candidates.push_back(temp);
                }
            }
            else
            {
                point_dis temp;
                temp.pnt = points[i];
                temp.distance = temp_sim_value;
                candidates.push_back(temp);
            }
        }
    }
}

void LineDetectorDNT::line_extraction(vector< Vector2<int> > points, vector<Vector4d> &produced_lines)
{
    vector< vector<Point> > lines;
    while(points.size() != 0)
    {
        bool end = false;
        vector< Vector2<int> > line;
        Vector2<int> start = points[0];
        Vector2<int> previous = points[0];
        do
        {
            line.push_back(previous);
            vector<candidate_point> candidates;
            find_candidate_points(points, start,  previous, line, candidates);
            // find best candidate to connect
            Point best_candidate;
            double error;
            find_best_candidate(image, candidates, line, start, previous, best_candidate, error);
            candidates.clear();
            if(error > 3)
            {
                end = true;
            }
            else
            {
                if(line.size() >= 4)
                {
                    double sum_error;
                    line_error(line, start, best_candidate, sum_error);
                    if(sum_error < 5)
                    {
                        previous = best_candidate;
                    }
                    else
                    {
                        if(line.size() <= 4 && line.size() != 0)
                        {
                            for(int i=0; i<line.size(); i++)
                            {
                                points.push_back(line[i]);
                            }
                            line.clear();
                        }
                        end = true;
                    }
                }
                else
                {
                    line.push_back(best_candidate);
                    previous = best_candidate;
                }
            }
            delete_point(previous, points);
        }
        while(!end);
        store_line(image, lines, line);
        line.clear();
    }
    //lines construction and export...
    for(int i = 0; i < lines.size(); i++)
    {
        Point point1,point2;
        double max_distance = 0;
        for(int j1 = 0; j1 < lines[i].size(); j1++)
        {
            for(int j2 = 0; j2 < lines[i].size(); j2++)
            {
                if(j1 != j2)
                {
                    double temp = points_distance(lines[i][j1], lines[i][j2]);
                    if(temp > max_distance)
                    {
                        point1 = lines[i][j1];
                        point2 = lines[i][j2];
                        max_distance = temp;
                    }
                }
            }
        }
        produced_lines.push_back(Vec4i(point1.x, point1.y, point2.x, point2.y));
    }
}
