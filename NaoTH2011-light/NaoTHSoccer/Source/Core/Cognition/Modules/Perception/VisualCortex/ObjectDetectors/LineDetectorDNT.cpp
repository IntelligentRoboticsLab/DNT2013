#include "LineDetectorDNT.h"

#include "Tools/Debug/Stopwatch.h"
#include <vector>

LineDetectorDNT::LineDetectorDNT()
{
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:dntlines", "mark the line candidates on the image", false);
}

void LineDetectorDNT::execute()
{
    lineSegments.clear();
    vector<Vector2d> scanPoints;

    scanLinesHorizontal(scanPoints);
//    scanLinesVertical(scanPoints);

    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:dntlines",
    for(int i = 0; i < (int) scanPoints.size(); i ++){
        CIRCLE_PX(ColorClasses::orange, (int) scanPoints[i].x -2 , (int) scanPoints[i].y -2, (int) 2);
    }
    );


}//end execute

void LineDetectorDNT::scanLinesHorizontal(vector<Vector2d> &linePoints)
{
    const int scanResolution = 15,
            scanStep = 2;
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
                if(thisPixelColor == ColorClasses::white)
                {
                    step = 2;
                }
                else if(thisPixelColor == ColorClasses::none)
                {
                }
                else
                {
                    step = 0;
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
                    if(whiteRatio > 0.50)
                    {
                        if(getFieldPercept().getLargestValidPoly(getArtificialHorizon()).isInside(Vector2d(j - ceil((numOfPixels)/2), i)))
                            linePoints.push_back(Vector2d(j - ceil((numOfPixels)/2), i));
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

void LineDetectorDNT::scanLinesVertical(vector<Vector2d> &linePoints)
{
    const int scanResolution = 15,
            scanStep = 2;
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
                if(thisPixelColor == ColorClasses::white)
                {
                    step = 2;
                }
                else if(thisPixelColor == ColorClasses::none)
                {
                }
                else
                {
                    step = 0;
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
                    if(whiteRatio > 0.50)
                    {
                        if(getFieldPercept().getLargestValidPoly(getArtificialHorizon()).isInside(Vector2d(i,j - floor((numOfPixels)/2))))
                            linePoints.push_back(Vector2d(i,j - floor((numOfPixels)/2)));
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
