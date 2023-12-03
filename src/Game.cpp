#include "Game.hpp"
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.hpp"
#include "WelcomeScreen.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

// Perform initialization, load fonts, read files, etc.
Game::Game() {

  // Read Data set window size.
  std::ifstream f(CONFIG_PATH);
  std::string temp;
  std::getline(f, temp);
  mData->mGameGlobals.mCols = std::stoi(temp);
  std::getline(f, temp);
  mData->mGameGlobals.mRows = std::stoi(temp);
  std::getline(f, temp);
  mData->mGameGlobals.mMines = std::stoi(temp);
  f.close();
  std::cout << "Cols: " << mData->mGameGlobals.mCols
            << " Rows: " << mData->mGameGlobals.mRows
            << " Mines: " << mData->mGameGlobals.mMines << std::endl;

  mData->mWindow.create(sf::VideoMode(SQUARE * mData->mGameGlobals.mCols,
                                      SQUARE * mData->mGameGlobals.mRows + 100),
                        TITLE);

  // Loading Font.
  mData->mAssetManager.LoadFont("font", FONT_PATH);

  mData->mStateManager.PushState(StateRef(new WelcomeScreen(mData)), false);

  Run();
}

void Game::Run() {
  while (mData->mWindow.isOpen()) {
    mData->mStateManager.ProcessStateChanges();
    mData->mStateManager.GetCurrentState()->ProcessEvent();
    mData->mStateManager.GetCurrentState()->Update();
    mData->mStateManager.GetCurrentState()->Draw();
  }
}
