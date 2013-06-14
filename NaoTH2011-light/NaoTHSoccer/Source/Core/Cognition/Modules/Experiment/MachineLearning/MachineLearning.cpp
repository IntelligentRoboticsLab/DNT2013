#include "MachineLearning.h"
#include <string>


MachineLearning::MachineLearning()
{
    REGISTER_DEBUG_COMMAND("machinelearning:evolution", "start/stop evolutionary method with current parameter settings", this);
    // TODO instantiate correct classes for used methods
    ltw = new LearnToWalk(getVirtualVision(),
                          getRobotPose(),
                          getCameraMatrix(),
                          getFrameInfo(),
                          getFieldInfo(),
                          getMotionRequest());
}

MachineLearning::~MachineLearning() {
    delete ltw;
}

void MachineLearning::execute() {
  if (ltw->method != NULL) {
    if(!finished) {
      // Until convergence, run the correct method
      if (!ltw->method->isFinished()) {
        ltw->run();
      }
    } else {
       // TODO save values
       std::cout<<"MachineLearning method " << ltw->method << " finished!"<<std::endl;
       finished = true;
       // exit(0);
    }
  }

}

void MachineLearning::executeDebugCommand(const std::string &command,
                                          const std::map<std::string, std::string> &arguments,
                                          std::ostream &outstream)
{
    if(!command.compare("machinelearning:method1"))
    {
        // enable/disable method
        if (arguments.find("on")!=arguments.end()) {

            unsigned int runningTime = ltw->theParameters.evolution.runningTime;
            ltw->method = new GA();

            if (!arguments.at("Task1").compare("on"))
            {
                ltw->theTests.push_back(LearnToWalk::Test(runningTime, Pose2D(0,10000,0)));
            }
            if (!arguments.at("Task2").compare("on"))
            {
                ltw->theTests.push_back(LearnToWalk::Test(runningTime/4, Pose2D(0,1000,0)));
            }
            if (!arguments.at("Task3").compare("on"))
            {
                ltw->theTests.push_back(LearnToWalk::Test(runningTime/4, Pose2D(Math::fromDegrees(30),500,0)));
            }

        } else if (arguments.find("off")!=arguments.end()) {
            finished = true;
        }
    }
    else if (!command.compare("machinelearning:getinfo"))
    {
        //TODO return info in the right format (in GitHub)
    }
}

