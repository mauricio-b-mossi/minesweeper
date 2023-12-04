#include "Game.hpp"
#include "Board.hpp"
#include "Constants.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "State.hpp"
#include <vector>

class GameScreen : public State {
public:
  GameScreen(GameDataRef gameDataRef);

  void Init() override;
  void Init(ExtrasRef extrasRef) override;

  void ProcessEvent() override;
  void Update() override;
  void Draw() override;

  void Pause() override;
  void Resume() override;

private:
  void SetAllSprites();
  void DrawAllTiles(std::string texture);
  void DrawOnlyMine();
  void DrawRevealedBoard();
  int GetIndex(int col, int row);

private:
  GameDataRef mData;
  bool mGameStarted = false;

  sf::Sprite mDebug;
  sf::Sprite mDigits;
  sf::Sprite mFaceHappy;
  sf::Sprite mFaceLose;
  sf::Sprite mFaceWin;
  sf::Sprite mFlag;
  sf::Sprite mLeaderboard;
  sf::Sprite mMine;
  sf::Sprite mNumber1;
  sf::Sprite mNumber2;
  sf::Sprite mNumber3;
  sf::Sprite mNumber4;
  sf::Sprite mNumber5;
  sf::Sprite mNumber6;
  sf::Sprite mNumber7;
  sf::Sprite mNumber8;
  sf::Sprite mPause;
  sf::Sprite mPlay;
  sf::Sprite mTileHidden;
  sf::Sprite mTileRevealed;
  std::string mPlayerName;

  Board mBoard;
};
