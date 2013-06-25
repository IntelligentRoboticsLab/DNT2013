/**
 * @file LedSymbols.cpp
 *
 * @author <a href="mailto:martius@informatik.hu-berlin.de">Martin Martius</a>
 * Implementation of class LedSymbols
 */


#include "LedSymbols.h"

using namespace naoth;

void LedSymbols::registerSymbols(xabsl::Engine& engine)
{
  // register the eyeLED enum
  engine.registerEnumElement("eyeLED", "eyeLED.off", 0);
  engine.registerEnumElement("eyeLED", "eyeLED.red", 1);
  engine.registerEnumElement("eyeLED", "eyeLED.green", 2);
  engine.registerEnumElement("eyeLED", "eyeLED.blue", 3);
  engine.registerEnumElement("eyeLED", "eyeLED.red_green", 4);

  engine.registerDecimalOutputSymbol("set_right_ear", &setRightEar, &getRightEar);
  engine.registerDecimalOutputSymbol("set_left_ear", &setLeftEar, &getLeftEar);

  engine.registerEnumeratedOutputSymbol("set_first_half_left_eye", "eyeLED", &setFirstHalfLeftEye, &getFirstHalfLeftEye);
  engine.registerEnumeratedOutputSymbol("set_second_half_left_eye", "eyeLED", &setSecondHalfLeftEye, &getSecondHalfLeftEye);
  engine.registerEnumeratedOutputSymbol("set_left_eye", "eyeLED", &setLeftEye, &getLeftEye);

  engine.registerEnumeratedOutputSymbol("set_first_half_right_eye", "eyeLED", &setFirstHalfRightEye, &getFirstHalfRightEye);
  engine.registerEnumeratedOutputSymbol("set_second_half_right_eye", "eyeLED", &setSecondHalfRightEye, &getSecondHalfRightEye);
  engine.registerEnumeratedOutputSymbol("set_right_eye", "eyeLED", &setRightEye, &getRightEye);

}//end registerSymbols



LedSymbols* LedSymbols::theInstance = NULL;

void LedSymbols::execute()
{
}


void LedSymbols::setRightEar(double id)
{

  if (id <= 10)
  {
    theInstance->lastREar = id;
    int counter = 0;
    for (int i = LEDData::EarRight0; i <= LEDData::EarRight324; i++)
    {
      if (counter < id)
      {
        theInstance->ledRequest.request.theMonoLED[i] = 1.0;
      }
      else
      {
        theInstance->ledRequest.request.theMonoLED[i] = 0.0;
      }
      counter++;
    }
  }

}//end setRightEar

double LedSymbols::getRightEar()
{
  return theInstance->lastREar;
}

void LedSymbols::setLeftEar(double id)
{
  id = floor(id + 0.5);
  if (id <= 10.0)
  {
    theInstance->lastLEar = id;
    int counter = 0;
    for (int i = LEDData::EarLeft0; i <= LEDData::EarLeft324; i++)
    {
      if (counter < id)
      {
        theInstance->ledRequest.request.theMonoLED[i] = 1.0;
      }
      else
      {
        theInstance->ledRequest.request.theMonoLED[i] = 0.0;
      }
      counter++;
    }
  }

}//end setLeftEar

double LedSymbols::getLeftEar()
{
  return theInstance->lastLEar;
}

void LedSymbols::setEye(LEDData::MultiLEDID from, LEDData::MultiLEDID to, int value)
{
  for (int i = from; i <= to; i++)
  {
    if(value == 0)
    {
      ledRequest.request.theMultiLED[i][LEDData::RED] = 0.0;
      ledRequest.request.theMultiLED[i][LEDData::GREEN] = 0.0;
      ledRequest.request.theMultiLED[i][LEDData::BLUE] = 0.0;
    }
    if (value == 1) // red
    {
      ledRequest.request.theMultiLED[i][LEDData::RED] = 1.0;
      ledRequest.request.theMultiLED[i][LEDData::GREEN] = 0.0;
      ledRequest.request.theMultiLED[i][LEDData::BLUE] = 0.0;
    }
    else if (value == 2) // green
    {
      ledRequest.request.theMultiLED[i][LEDData::RED] = 0.0;
      ledRequest.request.theMultiLED[i][LEDData::GREEN] = 1.0;
      ledRequest.request.theMultiLED[i][LEDData::BLUE] = 0.0;
    }
    else if (value == 3) // blue
    {
      ledRequest.request.theMultiLED[i][LEDData::RED] = 0.0;
      ledRequest.request.theMultiLED[i][LEDData::GREEN] = 0.0;
      ledRequest.request.theMultiLED[i][LEDData::BLUE] = 1.0;
    }
    else if (value == 4) // red_green
    {
      double red = 1.0;
      double green = 1.0;
      double blue = 0.0;

      ledRequest.request.theMultiLED[i][LEDData::RED]
        = (i % 2 == 0) ? red : 0.0;
      ledRequest.request.theMultiLED[i][LEDData::GREEN]
        = (i % 2 == 0) ? 0.0 : green;
      ledRequest.request.theMultiLED[i][LEDData::BLUE]
        = (i % 2 == 0) ? blue : 0.0;
    }
  }
}

// LEFT EYE
void LedSymbols::setLeftEye(int value)
{
  theInstance->lastFirstHalfLEye = value;
  theInstance->lastSecondHalfLEye = value;
  theInstance->setEye(LEDData::FaceLeft0, LEDData::FaceLeft315, value);
}
int LedSymbols::getLeftEye()
{
  return theInstance->lastFirstHalfLEye;
}

void LedSymbols::setFirstHalfLeftEye(int value)
{
  theInstance->lastFirstHalfLEye = value;
  theInstance->setEye(LEDData::FaceLeft0, LEDData::FaceLeft135, value);
}
int LedSymbols::getFirstHalfLeftEye()
{
  return theInstance->lastFirstHalfLEye;
}

void LedSymbols::setSecondHalfLeftEye(int value)
{
  theInstance->lastSecondHalfLEye = value;
  theInstance->setEye(LEDData::FaceLeft180, LEDData::FaceLeft315, value);
}
int LedSymbols::getSecondHalfLeftEye()
{
  return theInstance->lastSecondHalfLEye;
}

// RIGHT EYE
void LedSymbols::setRightEye(int value)
{
  theInstance->lastFirstHalfREye = value;
  theInstance->lastSecondHalfREye = value;
  theInstance->setEye(LEDData::FaceRight0, LEDData::FaceRight315, value);
}
int LedSymbols::getRightEye()
{
  return theInstance->lastFirstHalfREye;
}

void LedSymbols::setFirstHalfRightEye(int value)
{
  theInstance->lastFirstHalfREye = value;
  theInstance->setEye(LEDData::FaceRight0, LEDData::FaceRight135, value);
}
int LedSymbols::getFirstHalfRightEye()
{
  return theInstance->lastFirstHalfREye;
}

void LedSymbols::setSecondHalfRightEye(int value)
{
  theInstance->lastSecondHalfREye = value;
  theInstance->setEye(LEDData::FaceRight180, LEDData::FaceRight315, value);
}
int LedSymbols::getSecondHalfRightEye()
{
  return theInstance->lastSecondHalfREye;
}
