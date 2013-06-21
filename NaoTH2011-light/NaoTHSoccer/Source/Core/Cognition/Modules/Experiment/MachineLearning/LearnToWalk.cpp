/**
  * @file LearnToWalk.cpp
  *
  */

#include "LearnToWalk.h"

LearnToWalk::LearnToWalk(const naoth::VirtualVision &vv,
                         const naoth::InertialSensorData &isd,
                         const naoth::ButtonData &bd,
                         const RobotPose &rp,
                         const CameraMatrix &cm,
                         const naoth::FrameInfo &fi,
                         const FieldInfo &field,
                         const BodyState &bs,
                         MotionRequest &mq,
                         HeadMotionRequest &hmq)
: method(NULL),
  killCurrent(false),

  theVirtualVision(vv),
  theInertialSensorData(isd),
  theButtonData(bd),
  theRobotPose(rp),
  theCameraMatrix(cm),
  theFrameInfo(fi),
  theFieldInfo(field),
  theBodyState(bs),
  theMotionRequest(mq),
  theHeadMotionRequest(hmq),
  stateTime(0),
  fallenCount(0),
  uprightCount(0),
  lastChestButtonEventCounter(0)
{
  #define REG_WALK_PARAMETER(name, minv, maxv)\
  theIKParameterBounds[#name] = Vector2<double>(minv, maxv);\
    theIKParameterNames.push_back(#name);\
    theIKParameterValues[#name]

  REG_WALK_PARAMETER(bodyOffsetX, 0, 20) = 10;
//  REG_WALK_PARAMETER(walk.doubleSupportTime, 0);
  REG_WALK_PARAMETER(walk.singleSupportTime, 200, 500) = 300;
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
  REG_WALK_PARAMETER(walk.comHeight, 245, 265) = 260;
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

    lastTime = theFrameInfo.getTime();
    theTest = theTests.end();
    state = RESET;
}

void LearnToWalk::setMethod(std::string methodName)
{
  if(!methodName.compare("evolution")) {
    this->method = new GA(theParameters.evolution, theIKParameterValues, theIKParameterBounds);
    this->manualReset = theParameters.evolution.manualReset;
    this->iterationsToGetUp = theParameters.evolution.iterationsToGetUp;
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
  ltwInfo << "Current test:\n" << theTest->name << std::endl;
  return methodInfo + "\n" + ltwInfo.str();
}

void LearnToWalk::run()
{
  // Some localization head movements
  theHeadMotionRequest.id = HeadMotionRequest::search;
  theHeadMotionRequest.searchCenter = Vector3<double>(2000, 0, 0);
  theHeadMotionRequest.searchSize = Vector3<double>(1500, 2000, 0);

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

  // fallprotection
  if (abs(theInertialSensorData.data.y) > 0.4 && fallenCount < 30)
  {
    theMotionRequest.id = motion::dead; // fall down
    theMotionRequest.forced = true;
    return;
  }

  // killbutton
  if (theButtonData.eventCounter[naoth::ButtonData::Chest] > lastChestButtonEventCounter )
  {
    lastChestButtonEventCounter = theButtonData.eventCounter[naoth::ButtonData::Chest];
    killCurrent = true;
  }

  // If stopping condition for evaluation is met
  if(state == RUNTESTS && (stateTime + runningTime < theFrameInfo.getTime() ||
                            fallenCount > 10 || theTest == theTests.end() || killCurrent)) {
    stateTime = theFrameInfo.getTime();
    state = RESET;
  }

  switch(state) {
    case RESET:
      allTestsDone();    // Reset all, evaluate
      stateTime = theFrameInfo.getTime();
      state = GETTINGUP;
      break;
    case GETTINGUP:
      if (manualReset) {
        theMotionRequest.id = motion::stand;
        theMotionRequest.forced = true;
        if (stateTime + resettingTime < theFrameInfo.getTime()) {
          stateTime = theFrameInfo.getTime();
          state = PREPAREFORTESTS;
        }
      } else {
         switch (theBodyState.fall_down_state) {
           case BodyState::lying_on_front:
             theMotionRequest.id = motion::stand_up_from_front;
             uprightCount = 0;
             break;
           case BodyState::lying_on_back:
             theMotionRequest.id = motion::stand_up_from_back;
             uprightCount = 0;
             break;
           case BodyState::upright:
             uprightCount++;
             if (uprightCount > iterationsToGetUp)
             {
               std::cout << "Nao seen as upright." << std::endl;
               stateTime = theFrameInfo.getTime();
               state = PREPAREFORTESTS;
             }
             break;
           default:
             break;
         }
      }
      break;
    case PREPAREFORTESTS:
      {// stop trying to beam
        std::stringstream answer;
        std::map<std::string, std::string> args;
        args["off"] = "";
        DebugRequest::getInstance().executeDebugCommand("SimSparkController:beam", args, answer);
        theMotionRequest.id = motion::stand;
        theMotionRequest.forced = false;
        if (stateTime + standingTime < theFrameInfo.getTime()) {
          stateTime = theFrameInfo.getTime();
          lastTime = stateTime;
          state = RUNTESTS;
        }
        break;
      }
    case RUNTESTS:
      {
        Pose2D walkReq = theTest->update(theFrameInfo.getTime() - lastTime, mypos);
        lastTime = theFrameInfo.getTime();

        if (theTest->isFinished()) {
            theTest++;
        }

        theMotionRequest.id = motion::walk;
        theMotionRequest.walkRequest.target = walkReq;

        break;
      }
    default:
      break;
  }
}

void LearnToWalk::allTestsDone()
{
  double fitness = 0;
  for( std::list<LearnToWalk::Test>::iterator iter=theTests.begin(); iter!=theTests.end(); ++iter)
  {
    fitness += iter->fitness;
    iter->reset();
  }

  if (fallenCount > 3)
    fitness = std::max(0.0, fitness-1000);

  method->update(fitness);
  reset();
}

Pose2D LearnToWalk::getPosition()
{
  Pose2D mypos;
  //Vector4<double> mypos;
  if (theVirtualVision.data.find("mypos") != theVirtualVision.data.end())
  {
    // use debug infomation from simulation server
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
  //DebugRequest::getInstance().executeDebugCommand("SimSparkController:beam",args,answer);

  fallenCount = 0;
  uprightCount = 0;
  killCurrent = false;

  // Reset test iterator and tests themselves
  theTest = theTests.begin();
  lastTime = theFrameInfo.getTime();
}

LearnToWalk::Test::Test(std::string name, unsigned int maxTime, const Pose2D& walkReq, bool absolute)
: name(name),
  fitness(0),
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
    fitness = 0;
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

  fitness = getFitness();

  return theWalkRequest;
}

bool LearnToWalk::Test::isFinished() const
{
  return theTimeLeft <= 0;
}

double LearnToWalk::Test::getFitness() const
{
  if (absolute) {
    Pose2D difference = (theWalkRequest - (theStopPos - theStartPos));
    // TODO use orientation, perhaps?
    return pow( pow(difference.translation.x, 2.0) + pow(difference.translation.y, 2), 0.5);
  } else {
    return getDistance();
  }
}

double LearnToWalk::Test::getDistance() const
{
  return pow(pow(theStopPos.translation.x-theStartPos.translation.x, 2.0) +
             pow(theStopPos.translation.y-theStartPos.translation.y, 2.0), 0.5);
}

