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

#include <math.h>

class LearnToWalk
{
public:
    class Test
    {
    public:
      Test():started(false),theMaxTime(0),theTimeLeft(0), absolute(false){}
      Test(std::string name, unsigned int maxTime, const Pose2D& walkReq, bool absolute);

      Pose2D update(unsigned int time, const Pose2D& pos);
      bool isFinished() const;
      double getDistance() const;
      double getFitness() const;
      void reset();

      std::string name;
    private:
      bool started;
      int theMaxTime;
      int theTimeLeft;
      bool absolute;
      Pose2D theWalkRequest;

      Pose2D theStartPos;
      Pose2D theStopPos;
    };

    LearnToWalk(const naoth::VirtualVision& vv,
                const RobotPose& rp,
                const CameraMatrix& cm,
                const naoth::FrameInfo& fi,
                const FieldInfo& field,
                MotionRequest& mq);
    virtual ~LearnToWalk() {}
    virtual void run();

    void setMethod(std::string methodName);
    bool isFinished() const;
    std::string getInfo();

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

    Pose2D getPosition();
    std::list<Test>::iterator theTest;

};

#endif // LEARNTOWALK_H
