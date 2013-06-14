#include "MachineLearning.h"


MachineLearning::MachineLearning()
{
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
  if (ltw.method != NULL) {
    if(!finished) {
      // Until convergence, run the correct method
      if (!ltw.method.isFinished()) {
        ltw.method.run();
      }
    } else {
       // TODO save values
       std::cout<<"MachineLearning method " << ltw.method << " finished!"<<std::endl;
       finished = true;
       // exit(0);
    }
  }
}

void MachineLearning::executeDebugCommand(const std::string &command,
                                          const std::map<std::string, std::string> &arguments,
                                          std::ostream &outstream)
{
    if(strcmp(command, "machinelearning:method1") == 0)
    {
        // enable/disable method
        if (arguments.find("on")!=arguments.end()) {
            if (strcmp(arguments.at("Task1"),"on") == 0)
            {
                ltw.theTests.push_back(Test(runningTime, Pose2D(0,10000,0)));
            }
            if (strcmp(arguments.at("Task2"),"on") == 0)
            {
                ltw.theTests.push_back(Test(runningTime/4, Pose2D(0,1000,0)));
            }
            if (strcmp(arguments.at("Task3"),"on") == 0)
            {
                ltw.theTests.push_back(Test(runningTime/4, Pose2D(Math::fromDegrees(30),500,0)));
            }
            if (strcmp(arguments.at("Task4"),"on") == 0)
            {
                ltw.theTests.push_back(Test(runningTime/4, Pose2D(Math::fromDegrees(-30),500,0)));
            }

        } else if (arguments.find("off")) {
            finished = true;
        }
    }
    else if (strcmp(command,"machinelearning:getinfo") == 0)
    {
        //TODO return info in the right format (in GitHub)
    }
}

