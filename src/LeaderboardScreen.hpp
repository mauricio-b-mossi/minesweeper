#pragma once
#include "Game.hpp"
#include "State.hpp"
#include "Leaderboard.hpp"

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

};
