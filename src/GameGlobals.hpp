#pragma once
#include <string>

struct GameGlobals {
  int mCols;
  int mRows;
  int mMines;
  bool mCustom;
  bool mDebug;
  std::string mPlayerName = "Default";
};
