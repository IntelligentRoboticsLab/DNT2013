#include "MachineLearning.h"

MachineLearning::MachineLearning()
{
  REGISTER_DEBUG_COMMAND("machinelearning:evolution", "start/stop evolutionary method with current parameter settings", this);
  REGISTER_DEBUG_COMMAND("machinelearning:getinfo", "get info about a currently running machine learning method",  this);
  REGISTER_DEBUG_COMMAND("machinelearning:killcurrent", "kill current evaluation", this);
  REGISTER_DEBUG_COMMAND("machinelearning:getTaskList", "get all evaluation tasks", this);
  // TODO instantiate correct classes for used methods

  ltw = new LearnToWalk(getVirtualVision(),
                        getRobotPose(),
                        getCameraMatrix(),
                        getFrameInfo(),
                        getFieldInfo(),
                        getGyrometerData(),
                        getMotionRequest(),
                        getHeadMotionRequest());
  finished = false;
}

void MachineLearning::setTests(unsigned int runningtime)
{
  // tests[name] = Test( runningtime, Pose2D(rotation, translation_x, translation_y), absolute)
  // Simple running tests, absolute=false
  tests["forward"]       = LearnToWalk::Test("forward", runningtime, Pose2D(0,  10000,  0), false);
  tests["backward"]      = LearnToWalk::Test("backward", runningtime, Pose2D(0, -10000,  0), false);
  tests["right"]         = LearnToWalk::Test("right", runningtime, Pose2D(0,  0,    -10000), false);
  tests["left"]          = LearnToWalk::Test("left", runningtime, Pose2D(0,  0,     10000), false);
  tests["forwardleft"]   = LearnToWalk::Test("forwardleft" ,runningtime, Pose2D(0,  10000,  10000), false);
  tests["forwardright"]  = LearnToWalk::Test("forwardright", runningtime, Pose2D(0,  10000, -10000), false);
  tests["backwardleft"]  = LearnToWalk::Test("backwardleft", runningtime, Pose2D(0, -10000,  10000), false);
  tests["backwardright"] = LearnToWalk::Test("backwardright", runningtime, Pose2D(0, -10000, -10000), false);

  // GoToTarget tests, absolute=true
  for(int angle=-150; angle != 150; angle+=50) {
    std::stringstream methodname;
    double rads = Math::fromDegrees(angle);
    double x = cos(rads);
    double y = sin(rads);
    methodname << "x_" << std::setprecision(3) << x <<
                  "_y_" << std::setprecision(3) << y <<
                  "_angle_" << std::setprecision(3) << rads;
    tests[methodname.str()] = LearnToWalk::Test(methodname.str(), runningtime, Pose2D(rads, 10000 * x, 10000 * y), true);
  }
}

MachineLearning::~MachineLearning() {
    delete ltw;
}

void MachineLearning::execute() {
  if (ltw->method != NULL) {
    if(!finished) {
      // Until convergence, run the correct method
      if (!ltw->isFinished()) {
        ltw->run();
      }
    } else {
       // TODO save values
       std::cout<<"MachineLearning method " << ltw->method->name << " finished!"<<std::endl;
       finished = true;
       // exit(0);
    }
  }

}

void MachineLearning::executeDebugCommand(const std::string &command,
                                          const std::map<std::string, std::string> &arguments,
                                          std::ostream &outstream)
{
    if(!command.compare("machinelearning:evolution"))
    {
        // enable/disable method
        if (arguments.find("on")!=arguments.end()) {

            // TODO define tests based on arguments method
            // TODO create better structure (instead of sending strings around, more private vars etc)
            // TODO assign testing weights (emphasis on walking forward/backward etc) through runningtime
            //unsigned int runningTime = ltw->theParameters.evolution.runningTime;
            setTests(ltw->theParameters.evolution.runningTime);
            ltw->setMethod("evolution");
            ltw->theTests.clear();

            for(std::map<std::string, LearnToWalk::Test >::iterator test=tests.begin(); test!=tests.end(); test++)
            {
              if (!(arguments.find(test->name).compare("on"))) {
                ltw->theTests.push_back(test->second);
              }
            }
            finished = false;

        } else if (arguments.find("off")!=arguments.end()) {
            finished = true;
        }
    }
    else if (!command.compare("machinelearning:getinfo"))
    {
        if (ltw->method != NULL) {
            outstream << ltw->getInfo() << std::endl;
        } else {
            outstream << "No machine learning method selected." << std::endl;
        }
    }
    else if (!command.compare("machinelearning:killcurrent"))
    {
      ltw->killCurrent = true;
    } else if (!command.compare("machinelearning:testcurrent"))
    {

    } else if(!command.compare("machinelearning:getTaskList"))
    {
      for (std::map<std::string, LearnToWalk::Test >::iterator iter=tests.begin(); iter!=tests.end(); iter++)
      {
        outstream << iter->first << std::endl;
      }
    }
}

