#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H

#include <ModuleFramework/Module.h>

#include "Learners/GA.h"
#include "LearnToWalk.h"
#include "Representations/Infrastructure/VirtualVision.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Infrastructure/FieldInfo.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Motion/Request/MotionRequest.h"
#include <DebugCommunication/DebugCommandManager.h>

BEGIN_DECLARE_MODULE(MachineLearning)
  REQUIRE(FrameInfo)
  REQUIRE(FieldInfo)
  REQUIRE(VirtualVision)
  REQUIRE(RobotPose)
  REQUIRE(CameraMatrix)
  REQUIRE(BallModel)

  PROVIDE(MotionRequest)
END_DECLARE_MODULE(MachineLearning)

class MachineLearning : public MachineLearningBase, public DebugCommandExecutor
{
public:
    MachineLearning();
    ~MachineLearning();

    virtual void execute();
    virtual void executeDebugCommand(
            const std::string& command,
            const std::map<std::string, std::string>& arguments,
            std::ostream &outstreclassesam);
private:
    LearnToWalk *ltw;
    bool finished;
};

#endif // MACHINELEARNING_H
