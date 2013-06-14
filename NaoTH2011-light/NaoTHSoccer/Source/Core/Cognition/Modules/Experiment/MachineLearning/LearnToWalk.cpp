/**
  * @file LearnToWalk.cpp
  *
  */

#include "LearnToWalk.h"

LearnToWalk::LearnToWalk(const naoth::VirtualVision &vv,
                         const RobotPose &rp,
                         const CameraMatrix &cm,
                         const naoth::FrameInfo &fi,
                         const FieldInfo &field,
                         MotionRequest &mq)
:theVirtualVision(vv),
  theRobotPose(rp),
  theCameraMatrix(cm),
  theFrameInfo(fi),
  theFieldInfo(field),
  theMotionRequest(mq),
  lastResetTime(0),
  fallenCount(0)
{
  #define REG_WALK_PARAMETER(name, minv, maxv)\
  theIKParameterBounds[#name] = Vector2<double>(minv, maxv);\
    theIKParameterNames.push_back(#name);\
    theIKParameterValues[#name]

  REG_WALK_PARAMETER(bodyOffsetX, 0, 20) = 10;
//  REG_WALK_PARAMETER(walk.doubleSupportTime, 0);
  REG_WALK_PARAMETER(walk.singleSupportTime, 100, 500) = 400;
//  REG_WALK_PARAMETER(walk.bodyRollOffset, 0);
  REG_WALK_PARAMETER(walk.bodyPitchOffset, 0, 10) = 5;
  REG_WALK_PARAMETER(walk.ZMPOffsetY, -10, 10) =  0;
//  REG_WALK_PARAMETER(walk.ZMPOffsetSpeedX, 0.0);
//  REG_WALK_PARAMETER(walk.ZMPOffsetSpeedY, 0.0);
  REG_WALK_PARAMETER(walk.footOffsetY, -10, 10) = 0;
  REG_WALK_PARAMETER(walk.stepHeight, 5, 50) = 15;
//  REG_WALK_PARAMETER(walk.footPitchOffset, -5, 5, 0);
//  REG_WALK_PARAMETER(walk.footYawOffset, -10, 10, 0);
//  REG_WALK_PARAMETER(walk.leftFootRollOffset, 0.0);
//  REG_WALK_PARAMETER(walk.rightFootRollOffset, 0.0);
  REG_WALK_PARAMETER(walk.footCurveFactor, 5, 15) = 10;
  REG_WALK_PARAMETER(walk.maxStepLength, 50, 150) = 100.0;
//  REG_WALK_PARAMETER(walk.maxStepWidth, 20, 50) = 30.0;
//  REG_WALK_PARAMETER(walk.maxStepTurn, 20, 60) = 40.0;
  REG_WALK_PARAMETER(walk.comHeight, 220, 265) = 260;
//  REG_WALK_PARAMETER(walk.stiffness, 1);
  //REG_WALK_PARAMETER(walk.useArm, -1, 1) = 0;
//  REG_WALK_PARAMETER(walk.enableDynamicStabilizer, 0);
//  REG_WALK_PARAMETER(walk.leftHipRollSingleSupFactor, 1);
//  REG_WALK_PARAMETER(walk.rightHipRollSingleSupFactor, 1);
  //REG_WALK_PARAMETER(walk.comFootDiffTime, -50, 50) = 0;

  //REG_WALK_PARAMETER(arm.shoulderPitchInterialSensorRate, -10, 0) = -10;
  //REG_WALK_PARAMETER(arm.shoulderRollInterialSensorRate, -10, 0) = -10;
  //REG_WALK_PARAMETER(arm.maxSpeed, 10, 300) = 200;

//  REG_WALK_PARAMETER(dynamicStabilizer.inertialSensorFactorX, 100);
//  REG_WALK_PARAMETER(dynamicStabilizer.inertialSensorFactorY, -100);
//  REG_WALK_PARAMETER(dynamicStabilizer.inertialSensorMinX, 1.5); // in degrees
//  REG_WALK_PARAMETER(dynamicStabilizer.inertialSensorMinY, 1.5);
    this->method = NULL;

    lastTime = theFrameInfo.getTime();
    theTest = theTests.end();
}

void LearnToWalk::run()
{
    // TODO change
    unsigned int resettingTime = theParameters.evolution.resettingTime;
    unsigned int standingTime = theParameters.evolution.standingTime;
    unsigned int runningTime = theParameters.evolution.runningTime;

    // TODO use staggering to see if the nao is unstable. (Stop before actually falling).
    Vector3<double> mypos = getPosition();
    if ( mypos.z < NaoInfo::TibiaLength + NaoInfo::ThighLength ) {
      fallenCount++;
    } else {
      fallenCount = 0;
    }
    bool isFallenDown = (fallenCount > 3 && lastResetTime + resettingTime < theFrameInfo.getTime());

    // If stopping condition for evaluation is met
    if (lastResetTime + resettingTime + runningTime + standingTime < theFrameInfo.getTime()
      || isFallenDown || theTest == theTests.end() )
    {
        double fitness = 0;
        for( std::list<LearnToWalk::Test>::const_iterator iter=theTests.begin(); iter!=theTests.end(); ++iter)
        {
          fitness += iter->getDistance();
        }
        method->update(fitness);
        reset();
    }
    // else if during resetting time
    else if (lastResetTime + resettingTime > theFrameInfo.getTime())
    {
        theMotionRequest.id = motion::stand;
        theMotionRequest.forced = true;
    // else if during standing time
    } else if (lastResetTime + resettingTime + standingTime > theFrameInfo.getTime()) // Reset done
    {
      // stop trying to beam
      std::stringstream answer;
      std::map<std::string, std::string> args;
      args["off"] = "";
      DebugRequest::getInstance().executeDebugCommand("SimSparkController:beam", args, answer);
      theMotionRequest.id = motion::stand;
      theMotionRequest.forced = false;

      // try to stand up, I guess?
      // TODO stand up if needed

    } else
    // else, regular walk request
    {
      // run
        Vector2<double> currentPos(mypos.x, mypos.y);
        Pose2D walkReq = theTest->update(theFrameInfo.getTime() - lastTime, currentPos);
        theMotionRequest.id = motion::walk;
        theMotionRequest.walkRequest.target = walkReq;
    }
  }

Vector3<double> LearnToWalk::getPosition()
{
    Vector3<double> mypos;
    if (theVirtualVision.data.find("mypos") != theVirtualVision.data.end())
    {
      // use debug infomation from simulation server
      mypos = theVirtualVision.data.find("mypos")->second;
    } else
    {
      // use self location
      mypos.x = theRobotPose.translation.x;
      mypos.y = theRobotPose.translation.y;
      mypos.z = theCameraMatrix.translation.z;
    }

    return mypos;
}

double LearnToWalk::evaluate()
{
    double fitness =0;
    for( std::list<Test>::const_iterator iter=theTests.begin(); iter!=theTests.end(); ++iter)
    {
      fitness += iter->getDistance();
    }

    return fitness;
}

void LearnToWalk::reset()
{
  lastResetTime = theFrameInfo.getTime();

  // Reset position in simspark if needded
  std::stringstream answer;
  std::map<std::string,std::string> args;
  args["on"] = "";
  DebugRequest::getInstance().executeDebugCommand("SimSparkController:beam",args,answer);

  // TODO get up if necessary
  fallenCount = 0;

  // Reset test iterator
  theTest = theTests.begin();
}

LearnToWalk::Test::Test(unsigned int maxTime, const Pose2D& walkReq)
: started(false),
theMaxTime(maxTime),
theWalkRequest(walkReq)
{

}

Pose2D LearnToWalk::Test::update(unsigned int time, const Vector2<double>& pos)
{
  theMaxTime -= time;

  if ( !started )
  {
    theStartPos = pos;
    started = true;
  }

  theStopPos = pos;

  return theWalkRequest;
}


bool LearnToWalk::Test::isFinished() const
{
  return theMaxTime <= 0;
}

double LearnToWalk::Test::getFitness() const
{
    // Currently just the distance from the goal
    return getDistance();
}

double LearnToWalk::Test::getDistance() const
{
  return (theStopPos-theStartPos).abs();
}

