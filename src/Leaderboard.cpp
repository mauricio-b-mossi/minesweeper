#include "Leaderboard.hpp"
#include <string>
#include <vector>

LeaderBoardEntry::LeaderBoardEntry(std::string name, int timeStampSecond)
    : name(name), timeStampSeconds(timeStampSecond){};

LeaderBoard::LeaderBoard(int size) : mSize(size){};

void LeaderBoard::Insert(LeaderBoardEntry entry) {
  mLeaderBoard.push_back(entry);
  std::sort(mLeaderBoard.begin(), mLeaderBoard.end());
  std::cout << "Inserted item: " << entry.name << entry.timeStampSeconds << std::endl;
  std::cout << "LeaderBoard size: " << mLeaderBoard.size() << std::endl;
  if (mLeaderBoard.size() > mSize)
    mLeaderBoard.pop_back();
}
std::vector<LeaderBoardEntry> &LeaderBoard::GetLeaderBoard() {
  return mLeaderBoard;
}
