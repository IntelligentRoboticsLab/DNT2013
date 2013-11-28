/**
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu, Yuan</a>
 * @author <a href="mailto:mellmann@informatik.hu-berlin.de">Mellmann, Heinrich</a>
 */

#ifndef _PlatformBase_h_
#define _PlatformBase_h_

#include <string>
#include <iostream>

// This file has been touched by the documentation effort
namespace naoth
{

/**
* @brief This class is an interface.
*
* The PlatformBase holds (and provides access to)
* some basic information about the platform. 
* 
* ToDo: define Platform.
*/
class PlatformBase
{
public:
  PlatformBase(const std::string& platformName, unsigned int basicTimeStep)
    :
    platformName(platformName),
    theBasicTimeStep(basicTimeStep)
  {}

  virtual ~PlatformBase() {
    std::cout << "destruct PlatformBase" << std::endl;
  }

  virtual std::string getBodyID() const = 0;
  virtual std::string getBodyNickName() const = 0;
  virtual std::string getHeadNickName() const = 0;

  inline const std::string& getName() const { return platformName; }
  inline unsigned int getBasicTimeStep() const { return theBasicTimeStep; }

private:
  std::string platformName;
  unsigned int theBasicTimeStep;
};


}//end namespace naoth

#endif  // _PlatformBase_h_

