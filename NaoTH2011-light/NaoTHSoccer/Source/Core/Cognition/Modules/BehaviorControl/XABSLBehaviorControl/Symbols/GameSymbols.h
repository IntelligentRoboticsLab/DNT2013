/**
* @file GameSymbols.h
*
* @author <a href="mailto:mellmann@informatik.hu-berlin.de">Heinrich Mellmann</a>
* Definition of class GameSymbols
*/

#ifndef __GameSymbols_H_
#define __GameSymbols_H_

#include <ModuleFramework/Module.h>
#include <XabslEngine/XabslEngine.h>

// representations
#include "Representations/Infrastructure/GameData.h"
#include "Representations/Modeling/PlayerInfo.h"
#include <Representations/Infrastructure/FrameInfo.h>

BEGIN_DECLARE_MODULE(GameSymbols)
  REQUIRE(PlayerInfo);
  REQUIRE(FrameInfo);
END_DECLARE_MODULE(GameSymbols)

class GameSymbols: public GameSymbolsBase
{

public:
  GameSymbols()
    :
    playerInfo(getPlayerInfo()),
    theFrameInfo(getFrameInfo())
  {
      theInstance = this;
    };
  virtual ~GameSymbols(){}
  
  /** registers the symbols at an engine */
  void registerSymbols(xabsl::Engine& engine);

  virtual void execute();
private:

  static GameSymbols* theInstance;
  static bool getOwnKickOff();
  static double getPlayerNumber();
  static double getMsecsRemaining();
  static double getTimeSinceGameStateChanged();
  static int getPlayMode();
  static int getOpponentTeamColor();
  static int getOwnTeamColor();

  const PlayerInfo& playerInfo;
  const FrameInfo& theFrameInfo;

};//end class GameSymbols

#endif // __GameSymbols_H_
