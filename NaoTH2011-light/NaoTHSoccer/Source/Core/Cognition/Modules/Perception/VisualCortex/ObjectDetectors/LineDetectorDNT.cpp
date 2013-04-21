#include "LineDetectorDNT.h"

#include "Tools/Debug/Stopwatch.h"


LineDetectorDNT::LineDetectorDNT()
{
    DEBUG_REQUEST_REGISTER("ImageProcessor:LineDetectorDNT:dntlines", "mark the line candidates on the image", false);
}

void LineDetectorDNT::execute()
{
    lineSegments.clear();
    DEBUG_REQUEST("ImageProcessor:LineDetectorDNT:dntlines",
        CIRCLE_PX(ColorClasses::gray, (int) 100, (int) 100, (int) 20);
    );

}//end execute
