/**
 * @file LearnToWalk.h
 *
 * Acts as a wrapper for machine learning method evaluation when learning
 * how to walk.
 */

#ifndef LEARNTOWALK_H
#define LEARNTOWALK_H

#include "Representations/Infrastructure/VirtualVision.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Modeling/BodyState.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Infrastructure/FieldInfo.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Motion/Request/MotionRequest.h"

#include "MachineLearningParameters.h"
#include "Learners/MachineLearningMethod.h"
#include <DebugCommunication/DebugCommandManager.h>

REGISTER_DEBUG_COMMAND("machinelearning:evolution", "start/stop evolutionary method with current parameter settings", this)

class LearnToWalk
{
    class Test
    {
    public:
      Test():started(false),theMaxTime(0){}
      Test(unsigned int maxTime, const Pose2D& walkReq);

      Pose2D update(unsigned int time, const Vector2<double>& pos);
      bool isFinished() const;
      double getDistance() const;

    private:
      bool started;
      int theMaxTime;
      Vector2<double> theStartPos;
      Vector2<double> theStopPos;
      Pose2D theWalkRequest;
    };


public:
    LearnToWalk(const naoth::VirtualVision& vv,
                const RobotPose& rp,
                const CameraMatrix& cm,
                const naoth::FrameInfo& fi,
                const FieldInfo& field,
                MotionRequest& mq);
    virtual ~LearnToWalk();
    virtual void run();

private:
    const naoth::VirtualVision& theVirtualVision;
    const RobotPose& theRobotPose;
    const CameraMatrix& theCameraMatrix;
    const naoth::FrameInfo& theFrameInfo;
    const FieldInfo& theFieldInfo;
    MotionRequest& theMotionRequest;

    MachineLearningParameters theParameters;
    std::vector<std::string> parameterNames;
    std::map<std::string, Vector2<double> > parameterBounds;

    std::list<Test> theTests;
    std::list<Test>::iterator theTest;
    MachineLearningMethod* method;

    void reset();

    Vector3 getPosition();
};

#endif // LEARNTOWALK_H
