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
     } else
       // TODO save values
       finished = true;
     }

   std::cout<<"MachineLearning finished!"<<std::endl;
   exit(0);
  }
}

void MachineLearning::executeDebugCommand(const std::string &command,
                                          const std::map<std::string, std::string> &arguments,
                                          std::ostream &outstream)
{
    if(command == "machinelearning:evolution")
    {
        // enable/disable evolution
        if (arguments.find("on")) {
            ltw.method = Evolution();
        } else if (arguments.find("off")) {
            finished = true;
        }
    }
    else if (command == "")
    {

    }
}

