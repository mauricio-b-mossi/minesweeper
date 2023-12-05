#pragma once
#include "Game.hpp"
#include "Leaderboard.hpp"
#include "State.hpp"

// Basically on creation read file. Set score, clicks should cloade the
// leader board.
class LeaderboardScreen : public State {
public:
  LeaderboardScreen(GameDataRef gameDataRef);

  // Write to file if mHasWon.
  ~LeaderboardScreen();

  void Init() override;
  void Init(ExtrasRef extrasRef) override;

  void ProcessEvent() override;
  void Update() override;
  void Draw() override;

  void Pause() override;
  void Resume() override;

private:
  GameDataRef mData;
  LeaderBoard mLeaderboard;
  bool mHasWon;
  const float mHeight;
  const float mWidth;
  const float mOffsetX;
  const float mOffsetY;
  sf::Text mLeaderboardText;
  std::string ZeroPad(int number, int requiredDigits);
  int mTimeStamp;
};
