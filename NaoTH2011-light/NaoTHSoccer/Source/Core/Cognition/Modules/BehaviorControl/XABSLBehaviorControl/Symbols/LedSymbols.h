/**
* @file LedSymbols.h
*
* @author <a href="mailto:martius@informatik.hu-berlin.de">Martin Martius</a>
* Definition of class LedSymbols
*/

#ifndef __LedSymbols_H_
#define __LEdSymbols_H_

#include <ModuleFramework/Module.h>
#include <XabslEngine/XabslEngine.h>


#include "Tools/Math/Common.h"
#include <math.h>


// representations
#include "Representations/Infrastructure/LEDRequest.h"

BEGIN_DECLARE_MODULE(LedSymbols)
  PROVIDE(BehaviorLEDRequest)
END_DECLARE_MODULE(LedSymbols)

class LedSymbols: public LedSymbolsBase
{

public:
  LedSymbols()
    :
    ledRequest(getBehaviorLEDRequest())
  {
    theInstance = this;
    lastREar = 0.0;
    lastLEar = 0.0;
    lastFirstHalfLEye = 0;
    lastSecondHalfLEye = 0;
    lastFirstHalfREye = 0;
    lastSecondHalfREye = 0;
};
  virtual ~LedSymbols(){}
  
  /** registers the symbols at an engine */
  void registerSymbols(xabsl::Engine& engine);

  virtual void execute();
private:

  static LedSymbols* theInstance;

  // representations
  LEDRequest& ledRequest;

  // helper
  void setEye(naoth::LEDData::MultiLEDID from, naoth::LEDData::MultiLEDID to, int value);

  // setter and getter

  static void setRightEar(double id);
  static void setLeftEar(double id);

  static void setFirstHalfLeftEye(int value);
  static void setSecondHalfLeftEye(int value);
  static void setLeftEye(int value);

  static void setFirstHalfRightEye(int value);
  static void setSecondHalfRightEye(int value);
  static void setRightEye(int value);

  double lastREar;
  double lastLEar;
  int lastFirstHalfREye;
  int lastSecondHalfREye;
  int lastFirstHalfLEye;
  int lastSecondHalfLEye;

  static double getRightEar();
  static double getLeftEar();
  static int getFirstHalfLeftEye();
  static int getSecondHalfLeftEye();
  static int getLeftEye();
  static int getFirstHalfRightEye();
  static int getSecondHalfRightEye();
  static int getRightEye();

};//end class LedSymbols

#endif // __LedSymbols_H_
