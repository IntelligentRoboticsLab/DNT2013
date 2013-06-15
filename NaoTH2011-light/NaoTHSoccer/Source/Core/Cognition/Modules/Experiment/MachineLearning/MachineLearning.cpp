#include "MachineLearning.h"
#include <string>


MachineLearning::MachineLearning()
{
    REGISTER_DEBUG_COMMAND("machinelearning:evolution", "start/stop evolutionary method with current parameter settings", this);
    REGISTER_DEBUG_COMMAND("machinelearning:getinfo", "get info about a currently running machine learning method",  this)
    // TODO instantiate correct classes for used methods


    ltw = new LearnToWalk(getVirtualVision(),
                          getRobotPose(),
                          getCameraMatrix(),
                          getFrameInfo(),
                          getFieldInfo(),
                          getMotionRequest());
    finished = false;
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

            // TODO set running time based on tests instead of other way around
            // TODO define tests based on arguments method
            // TODO create better structure (instead of sending strings around, more private vars etc)
            unsigned int runningTime = ltw->theParameters.evolution.runningTime;
            ltw->setMethod("evolution");

            if (!arguments.at("Task1").compare("on"))
            {
                ltw->theTests.push_back(LearnToWalk::Test(runningTime/2, Pose2D(0,10000,0)));
            }
            if (!arguments.at("Task2").compare("on"))
            {
                ltw->theTests.push_back(LearnToWalk::Test(runningTime/2, Pose2D(0,0,10000)));
            }
            if (!arguments.at("Task3").compare("on"))
            {
                ltw->theTests.push_back(LearnToWalk::Test(runningTime/4, Pose2D(Math::fromDegrees(30),500,0)));
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
}

