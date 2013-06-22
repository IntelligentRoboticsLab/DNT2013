/**
* @file VisualMapping.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* Definition of class VisualMapping
*/

#ifndef VISUALMAPPING_H
#define VISUALMAPPING_H

#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/FieldInfo.h"
#include "Representations/Infrastructure/SoundData.h"

#include "Representations/Perception/BallPercept.h"
#include "Representations/Perception/PlayersPercept.h"
#include "Representations/Modeling/BallModel.h"

#include "Representations/Motion/Request/HeadMotionRequest.h"
#include "Representations/Motion/Request/MotionRequest.h"
#include "Representations/Motion/MotionStatus.h"

#include "Representations/Modeling/AttentionModel.h"

class VisualMapping
{
public:
    VisualMapping();
    ~VisualMapping();
    /*
     *Performs a turn in the robot's position and
     *scans the images for color transitions, storing
     *the information.
     */
    void MapEnvironment();
};

#endif // VISUALMAPPING_H
