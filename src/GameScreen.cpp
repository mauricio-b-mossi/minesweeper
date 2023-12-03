#include "GameScreen.hpp"
#include "Constants.hpp"
#include "Game.hpp"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "State.hpp"
#include <ctype.h>
#include <iostream>
#include <memory>
#include <vector>

GameScreen::GameScreen(GameDataRef gameDataRef)
    : mData(gameDataRef),
      mBoard(mData->mGameGlobals.mCols, mData->mGameGlobals.mRows,
             mData->mGameGlobals.mMines){};

void GameScreen::Init(){};

void GameScreen::Init(ExtrasRef extrasRef) {
  std::cout << extrasRef->size() << std::endl;
  if (extrasRef->size() > 0) {
    mPlayerName = extrasRef->at(0);
    std::cout << mPlayerName << std::endl;
    mPname.setFont(mData->mAssetManager.GetFont("font"));
    mPname.setString(mPlayerName);
  }
};

void GameScreen::ProcessEvent() {
  sf::Event ev;
  while (mData->mWindow.pollEvent(ev)) {
    if (ev.type == sf::Event::Closed) {
      mData->mWindow.close();
    }
  }
};

void GameScreen::Update(){};

void GameScreen::Draw() {
  mData->mWindow.clear();
  mData->mWindow.draw(mPname);
  mData->mWindow.display();
};

void GameScreen::Pause(){};
void GameScreen::Resume(){};
