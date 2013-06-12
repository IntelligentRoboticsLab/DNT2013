#ifndef MACHINELEARNING_H
#define MACHINELEARNING_H

#include <ModuleFramework/Module.h>
#include "MachineLearningParameters.h"

#include "Representations/Infrastructure/VirtualVision.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Infrastructure/FieldInfo.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Motion/Request/MotionRequest.h"


BEGIN_DECLARE_MODULE(MachineLearning)
  REQUIRE(FrameInfo)
  REQUIRE(FieldInfo)
  REQUIRE(VirtualVision)
  REQUIRE(RobotPose)
  REQUIRE(CameraMatrix)
  REQUIRE(BallModel)

  PROVIDE(MotionRequest)
END_DECLARE_MODULE(MachineLearning)

class MachineLearning : public MachineLearningBase
{

public:
    MachineLearning();
    ~MachineLearning();
    virtual void execute();

    //const MachineLearningParameters& getParameters() { return theParameters; }
private:
    MachineLearningParameters theParameters;
    //const MachineLearningParameters::Method1& theMethod1Parameters;
    //const MachineLearningParameters::Method2& theMethod2Parameters;

};



#endif // MACHINELEARNING_H
