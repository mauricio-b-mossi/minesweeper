#include "Game.hpp"
#include <fstream>
#include <iostream>
#include <string>

#include "Constants.hpp"
#include "StateManager.hpp"
#include "WelcomeScreen.hpp"
#include "GameScreen.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

// Perform initialization, load fonts, read files, etc.
Game::Game() {

  LoadConfigurations();

  mData->mWindow.create(sf::VideoMode(SQUARE * mData->mGameGlobals.mCols,
                                      SQUARE * mData->mGameGlobals.mRows + 100),
                        TITLE);

  LoadAssets();

  mData->mStateManager.PushState(StateRef(new GameScreen(mData)), false);

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

void Game::LoadAssets() {
  // Loading Font.
  mData->mAssetManager.LoadFont("font", FONT_PATH);

  // Loading Sprites.
  mData->mAssetManager.LoadTexture("debug", DEBUG);
  mData->mAssetManager.LoadTexture("digits", DIGITS);
  mData->mAssetManager.LoadTexture("face_happy", FACE_HAPPY);
  mData->mAssetManager.LoadTexture("face_lose", FACE_LOSE);
  mData->mAssetManager.LoadTexture("face_win", FACE_WIN);
  mData->mAssetManager.LoadTexture("flag", FLAG);
  mData->mAssetManager.LoadTexture("leaderboard", LEADERBOARD);
  mData->mAssetManager.LoadTexture("mine", MINE);
  mData->mAssetManager.LoadTexture("number_1", NUMBER_1);
  mData->mAssetManager.LoadTexture("number_2", NUMBER_2);
  mData->mAssetManager.LoadTexture("number_3", NUMBER_3);
  mData->mAssetManager.LoadTexture("number_4", NUMBER_4);
  mData->mAssetManager.LoadTexture("number_5", NUMBER_5);
  mData->mAssetManager.LoadTexture("number_6", NUMBER_6);
  mData->mAssetManager.LoadTexture("number_7", NUMBER_7);
  mData->mAssetManager.LoadTexture("number_8", NUMBER_8);
  mData->mAssetManager.LoadTexture("pause", PAUSE);
  mData->mAssetManager.LoadTexture("play", PLAY);
  mData->mAssetManager.LoadTexture("tile_hidden", TILE_HIDDEN);
  mData->mAssetManager.LoadTexture("tile_revealed", TILE_REVEALED);
}

void Game::LoadConfigurations() {
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
}
