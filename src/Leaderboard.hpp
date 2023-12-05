#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct LeaderBoardEntry {
  LeaderBoardEntry(std::string name, int timeStampSeconds);
  std::string name;
  int timeStampSeconds;
  bool operator<(LeaderBoardEntry &rhs){
    return this->timeStampSeconds < rhs.timeStampSeconds;
  };
};

struct LeaderBoard {

  LeaderBoard(int size);

  // Not most efficient but rn im fried.
  void Insert(LeaderBoardEntry entry);

  std::vector<LeaderBoardEntry> &GetLeaderBoard();

  int mSize;

private:
  std::vector<LeaderBoardEntry> mLeaderBoard{};
};
