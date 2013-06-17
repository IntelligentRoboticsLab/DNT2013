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
  REG_WALK_PARAMETER(walk.maxStepLength, 50, 120) = 100.0;
  REG_WALK_PARAMETER(walk.maxStepWidth, 20, 50) = 30.0;
  REG_WALK_PARAMETER(walk.maxStepTurn, 20, 60) = 40.0;
  REG_WALK_PARAMETER(walk.comHeight, 230, 265) = 260;
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

void LearnToWalk::setMethod(std::string methodName)
{
    if(!methodName.compare("evolution")) {
        this->method = new GA(theParameters.evolution, theIKParameterValues, theIKParameterBounds);
    } else  {
        std::cout << "Trying to use unknown method '"  << methodName << "'.";
    }
}

bool LearnToWalk::isFinished() const
{
    return method->isFinished();
}

std::string LearnToWalk::getInfo()
{
    std::string methodInfo = method->getInfo();
    std::stringstream ltwInfo;
    ltwInfo << "Current test: " << theTest->name << std::endl;
    return methodInfo + "\n" + ltwInfo.str();
}

void LearnToWalk::run()
{
    // TODO change
    unsigned int resettingTime = theParameters.evolution.resettingTime;
    unsigned int standingTime = theParameters.evolution.standingTime;
    unsigned int runningTime = theParameters.evolution.runningTime * theTests.size();

    // TODO use staggering to see if the nao is unstable. (Stop before actually falling).

    Pose2D mypos = getPosition();
    if ( theCameraMatrix.translation.z < NaoInfo::TibiaLength + NaoInfo::ThighLength ) {
      fallenCount++;
    } else {
      fallenCount = 0;
    }
    bool isFallenDown = (fallenCount > 3 && lastResetTime + resettingTime < theFrameInfo.getTime());

    // If stopping condition for evaluation is met
    if (lastResetTime + resettingTime + runningTime + standingTime < theFrameInfo.getTime()
      || isFallenDown || theTest == theTests.end() )
    {
        //if (theMotionRequest.emergencyStop) { std::cout << "Emergency stop!" << std::endl; }

        double fitness = 0;
        for( std::list<LearnToWalk::Test>::iterator iter=theTests.begin(); iter!=theTests.end(); ++iter)
        {
          fitness += iter->getDistance();
          iter->reset();
        }

        if (fallenCount > 3)
          fitness = std::max(0.0, fitness-1000);

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
        Pose2D walkReq = theTest->update(theFrameInfo.getTime() - lastTime, mypos);
        lastTime = theFrameInfo.getTime();

        if (theTest->isFinished()) {
            theTest++;
        }

        theMotionRequest.id = motion::walk;
        theMotionRequest.walkRequest.target = walkReq;
    }
  }

Pose2D LearnToWalk::getPosition()
{
  Pose2D mypos;
  //Vector4<double> mypos;
  if (theVirtualVision.data.find("mypos") != theVirtualVision.data.end())
  {
    // use debug infomation from simulation server
    std::cout << theVirtualVision.data.at("mypos") << std::endl;
    mypos = Pose2D(0,0,0); // theVirtualVision.data.find("mypos")->second );
  } else
  {
    // use self location
    mypos = theRobotPose;
  }

  return mypos;
}

double LearnToWalk::evaluate()
{
    double num = 1;
    double den = 0;
    for( std::list<Test>::const_iterator iter=theTests.begin(); iter!=theTests.end(); ++iter)
    {
      num *= iter->getDistance();
      den += iter->getDistance();
    }

    return num/den;
}

void LearnToWalk::reset()
{
  // Reset position in simspark if needded
  std::stringstream answer;
  std::map<std::string,std::string> args;
  args["on"] = "";
  DebugRequest::getInstance().executeDebugCommand("SimSparkController:beam",args,answer);

  // TODO get up if necessary
  fallenCount = 0;

  // Reset test iterator and tests themselves
  theTest = theTests.begin();
  lastResetTime = theFrameInfo.getTime();
  lastTime = theFrameInfo.getTime();
}

LearnToWalk::Test::Test(std::string name, unsigned int maxTime, const Pose2D& walkReq, bool absolute)
: name(name),
started(false),
theMaxTime(maxTime),
theTimeLeft(maxTime),
absolute(absolute),
theWalkRequest(walkReq)
{

}

void LearnToWalk::Test::reset() {
    theTimeLeft = theMaxTime;
    started = false;
}

Pose2D LearnToWalk::Test::update(unsigned int time, const Pose2D& pos)
{
  theTimeLeft -= time;

  if ( !started )
  {
    theStartPos = pos;
    started = true;
  }

  theStopPos = pos;

  if ( absolute )
  {
    return (theStartPos + theWalkRequest) - pos;
  }

  return theWalkRequest;
}

bool LearnToWalk::Test::isFinished() const
{
  return theTimeLeft <= 0;
}

double LearnToWalk::Test::getFitness() const
{
    //currently just walked distance
    return getDistance();
}

double LearnToWalk::Test::getDistance() const
{
  return pow(pow(theStopPos.translation.x-theStartPos.translation.x, 2.0) +
             pow(theStopPos.translation.y-theStartPos.translation.y, 2.0), 0.5);
}

