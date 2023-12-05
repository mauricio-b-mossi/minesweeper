#include "LeaderboardScreen.hpp"
#include "Constants.hpp"
#include "Game.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "State.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

LeaderboardScreen::LeaderboardScreen(GameDataRef gameDataRef)
    : mData(gameDataRef), mLeaderboard(5),
      mHeight(((SQUARE / 2.f * mData->mGameGlobals.mRows) + 50)),
      mWidth((SQUARE / 2.f * mData->mGameGlobals.mCols)),
      mOffsetX((mData->mWindow.getSize().x / 2.f) - (mWidth / 2.f)),
      mOffsetY((mData->mWindow.getSize().y / 2.f) - (mHeight / 2.f)) {
  mLeaderboardText.setFont(mData->mAssetManager.GetFont("font"));
  mLeaderboardText.setString("LEADERBOARD");
  mLeaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
  mLeaderboardText.setCharacterSize(20);
  mLeaderboardText.setPosition(sf::Vector2f(
      mOffsetX + mWidth / 2 - mLeaderboardText.getLocalBounds().width / 2.f,
      mOffsetY + mHeight / 2 - 120 -
          mLeaderboardText.getLocalBounds().height / 2.f));
};

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

  std::ifstream f(LEADERBOARD_PATH);
  if (!f.is_open()) {
    std::cout << "Could not open leaderboard at " << LEADERBOARD_PATH
              << "\nCreating leaderboard file." << std::endl;
    std::ofstream f(LEADERBOARD_PATH);
    f.close();
    return Init(std::move(extrasRef));
  }

  mHasWon = true;
  mTimeStamp = std::stoi(extrasRef->at(0));

  mLeaderboard.Insert(
      LeaderBoardEntry(mData->mGameGlobals.mPlayerName, mTimeStamp));

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

  std::cout << "YES EXTRAS" << std::endl;
  for (auto &el : mLeaderboard.GetLeaderBoard()) {
    std::cout << el.name << el.timeStampSeconds << std::endl;
  }
};

void LeaderboardScreen::Init() {
  std::ifstream f(LEADERBOARD_PATH);

  if (!f.is_open()) {
    std::cout << "Could not open leaderboard at " << LEADERBOARD_PATH
              << "\nCreating leaderboard file." << std::endl;
    std::ofstream f(LEADERBOARD_PATH);
    f.close();
    return Init();
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
  std::cout << "NO EXTRAS" << std::endl;
  for (auto &el : mLeaderboard.GetLeaderBoard()) {
    std::cout << el.name << el.timeStampSeconds << std::endl;
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
      f << ZeroPad(el.timeStampSeconds / 60, 2) << ':'
        << ZeroPad(el.timeStampSeconds % 60, 2) << ',' << el.name << std::endl;

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

void LeaderboardScreen::Draw() {

  sf::RectangleShape overlay(
      sf::Vector2f(mData->mWindow.getSize().x, mData->mWindow.getSize().y));

  overlay.setFillColor(sf::Color(0, 0, 0, 25));

  sf::RectangleShape rect(sf::Vector2f(mWidth, mHeight));

  rect.setPosition(sf::Vector2f(mOffsetX, mOffsetY));
  rect.setFillColor(sf::Color::Blue);

  mData->mWindow.draw(overlay);
  mData->mWindow.draw(rect);

  // Display text
  mData->mWindow.draw(mLeaderboardText);

  std::ostringstream temp;
  int position = 1;
  for (auto &el : mLeaderboard.GetLeaderBoard()) {
    if (mHasWon && (mData->mGameGlobals.mPlayerName == el.name) &&
        (mTimeStamp == el.timeStampSeconds)) {
      temp << position << ".\t" << ZeroPad(el.timeStampSeconds / 60, 2) << ':'
           << ZeroPad(el.timeStampSeconds % 60, 2) << '\t' << el.name << '*'
           << "\n\n";

    } else {
      temp << position << ".\t" << ZeroPad(el.timeStampSeconds / 60, 2) << ':'
           << ZeroPad(el.timeStampSeconds % 60, 2) << '\t' << el.name << "\n\n";
    }
    position++;
  }

  sf::Text leaderboardEntries;
  leaderboardEntries.setString(temp.str());
  leaderboardEntries.setFont(mData->mAssetManager.GetFont("font"));
  leaderboardEntries.setCharacterSize(18);
  leaderboardEntries.setStyle(sf::Text::Bold);
  leaderboardEntries.setPosition(
      mOffsetX + mWidth / 2 - leaderboardEntries.getLocalBounds().width / 2.f,
      mOffsetY + mHeight / 2 + 20 -
          leaderboardEntries.getLocalBounds().height / 2.f);

  mData->mWindow.draw(leaderboardEntries);

  mData->mWindow.display();
}

void LeaderboardScreen::Update() {}

void LeaderboardScreen::Pause() {}
void LeaderboardScreen::Resume() {}

std::string LeaderboardScreen::ZeroPad(int number, int requiredDigits) {
  std::string num = std::to_string(number);
  if (num.length() < requiredDigits) {
    std::string temp;
    for (int i = 0; i < requiredDigits - num.length(); i++)
      temp.push_back('0');
    return temp + num;
  }
  return num;
}
