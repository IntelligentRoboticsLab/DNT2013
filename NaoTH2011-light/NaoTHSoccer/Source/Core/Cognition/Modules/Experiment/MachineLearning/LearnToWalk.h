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
#include "Learners/GA.h"
#include "Tools/NaoInfo.h"
#include "Tools/Debug/DebugRequest.h"
#include <DebugCommunication/DebugCommandManager.h>


class LearnToWalk
{
public:
    class Test
    {
    public:
      Test():started(false),theMaxTime(0){}
      Test(unsigned int maxTime, const Pose2D& walkReq);

      Pose2D update(unsigned int time, const Vector2<double>& pos);
      bool isFinished() const;
      double getDistance() const;
      double getFitness() const;

    private:
      bool started;
      int theMaxTime;
      Vector2<double> theStartPos;
      Vector2<double> theStopPos;
      Pose2D theWalkRequest;
    };

    LearnToWalk(const naoth::VirtualVision& vv,
                const RobotPose& rp,
                const CameraMatrix& cm,
                const naoth::FrameInfo& fi,
                const FieldInfo& field,
                MotionRequest& mq);
    virtual ~LearnToWalk() {}
    virtual void run();
    MachineLearningMethod* method;
    MachineLearningParameters theParameters;
    std::list<Test> theTests;

private:
    const naoth::VirtualVision& theVirtualVision;
    const RobotPose& theRobotPose;
    const CameraMatrix& theCameraMatrix;
    const naoth::FrameInfo& theFrameInfo;
    const FieldInfo& theFieldInfo;
    MotionRequest& theMotionRequest;

    unsigned int lastResetTime;
    unsigned int lastTime;
    unsigned int fallenCount;

    std::vector<std::string> theIKParameterNames;
    std::map<std::string, Vector2<double> > theIKParameterBounds;
    std::map<std::string, double> theIKParameterValues;
    void reset();
    double evaluate();

    Vector3<double> getPosition();
    std::list<Test>::iterator theTest;

};

#endif // LEARNTOWALK_H
