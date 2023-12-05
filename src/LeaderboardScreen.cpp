#include "LeaderboardScreen.hpp"
#include "Constants.hpp"
#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "State.hpp"
#include <fstream>
#include <iostream>
#include <string>

LeaderboardScreen::LeaderboardScreen(GameDataRef gameDataRef)
    : mData(gameDataRef), mLeaderboard(5){};

// If Extras, extrasRef->at(0) constists of the timeStampSeconds.
// the name of the player is a GlobalProperty of the Game, hence it
// is stored in mData->GameGlobals.mPlayerName.
void LeaderboardScreen::Init(ExtrasRef extrasRef) {

  if (extrasRef->size() < 1) {
    std::cout << "Error LeaderboardScreen: Insuficient Extras Provided. Pass "
                 "timeStampSeconds as extrasRef->at(0)."
              << std::endl;
    return Init();
  }

  mHasWon = true;

  mLeaderboard.Insert(LeaderBoardEntry(mData->mGameGlobals.mPlayerName,
                                       std::stoi(extrasRef->at(0))));

  std::ifstream f("LEADERBOARD_PATH");
  if (!f.is_open()) {
    std::cout << "Could not open leaderboard, creating leaderboard file."
              << std::endl;
    std::ofstream f("LEADERBOARD_PATH");
    f.close();
  }
  std::string temp;
  int idxOfColon;
  int idxOfComma;
  while (std::getline(f, temp)) {
    idxOfColon = temp.find_first_of(':');
    idxOfComma = temp.find_first_of(',');
    std::cout << "Minutes: " << temp.substr(0, idxOfColon) << " Seconds: "
              << temp.substr(idxOfColon + 1, idxOfComma - idxOfColon - 1)
              << " Name: " << temp.substr(idxOfComma + 1, temp.size())
              << std::endl;
    int timeStamp =
        std::stoi(temp.substr(0, idxOfColon)) * 60 +
        std::stoi(temp.substr(idxOfColon + 1, idxOfComma - idxOfColon - 1));
    mLeaderboard.Insert(
        LeaderBoardEntry(temp.substr(idxOfComma + 1, temp.size()), timeStamp));
  }
};

void LeaderboardScreen::Init() {
  std::ifstream f("LEADERBOARD_PATH");
  if (!f.is_open()) {
    std::cout << "Could not open leaderboard, creating leaderboard file."
              << std::endl;
    std::ofstream f("LEADERBOARD_PATH");
    f.close();
  }
  std::string temp;
  int idxOfColon;
  int idxOfComma;
  while (std::getline(f, temp)) {
    idxOfColon = temp.find_first_of(':');
    idxOfComma = temp.find_first_of(',');
    std::cout << "Minutes: " << temp.substr(0, idxOfColon) << " Seconds: "
              << temp.substr(idxOfColon + 1, idxOfComma - idxOfColon - 1)
              << " Name: " << temp.substr(idxOfComma + 1, temp.size())
              << std::endl;
    int timeStamp =
        std::stoi(temp.substr(0, idxOfColon)) * 60 +
        std::stoi(temp.substr(idxOfColon + 1, idxOfComma - idxOfColon - 1));
    mLeaderboard.Insert(
        LeaderBoardEntry(temp.substr(idxOfComma + 1, temp.size()), timeStamp));
  }
};

LeaderboardScreen::~LeaderboardScreen() {
  if (mHasWon) {
    std::ofstream f(LEADERBOARD_PATH);
    if (!f.is_open()) {
      std::cout << "Error LeaderboardScreen: Failed to open "
                << LEADERBOARD_PATH << " for writing." << std::endl;
      return;
    }
    for (auto &el : mLeaderboard.GetLeaderBoard()) {
      f << el.timeStampSeconds / 60 << ':' << el.timeStampSeconds % 60 << ','
        << el.name << std::endl;
      std::cout << "Wrote Successfully to " << LEADERBOARD_PATH << std::endl;
    }
  }
}

void LeaderboardScreen::ProcessEvent() {
  sf::Event event;
  while (mData->mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mData->mWindow.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      mData->mStateManager.PopState();
    }
  }
}

void LeaderboardScreen::Draw() {}

void LeaderboardScreen::Update() {}

void LeaderboardScreen::Pause(){}
void LeaderboardScreen::Resume(){}
