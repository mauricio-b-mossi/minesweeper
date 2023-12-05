#pragma once
#include "Game.hpp"
#include "Board.hpp"
#include "Constants.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "State.hpp"
#include "Stopwatch.hpp"
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
  void SetConstantSprites();
  void DrawAllTiles(std::string texture);
  void DrawOnlyMine();
  void DrawRevealedBoard();
  void DrawControls(bool custom);
  void DrawBoardState(bool flags);
  void DrawCounter();
  void DrawStopwatch(int offset, int padding, int number);
  bool IsPaused();
  int GetIndex(int col, int row);
  void Solve();

private:
  GameDataRef mData;
  bool mIsPlaying = false;
  bool mIsDebugging = false;
  Stopwatch mStopWatch;

  // Pos for ease of event handling since buttons are 64 x 64.
  int mXposDebug;
  int mYposDebug;

  int mXposPlayPause;
  int mYposPlayPause;

  int mXposLeaderboard;
  int mYposLeaderboard;

  int mXposFace;
  int mYposFace;

  int mYactionRow;

  int mYposDigit;

  sf::Sprite mLeaderboard;
  std::string mPlayerName;

  Board mBoard;
};
