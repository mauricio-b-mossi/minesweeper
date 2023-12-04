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
  if (extrasRef->size() > 0) {
    mPlayerName = extrasRef->at(0);
  }
};

void GameScreen::ProcessEvent() {
  sf::Event event;
  while (mData->mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mData->mWindow.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        int col = event.mouseButton.x / 32;
        int row = event.mouseButton.y / 32;

        // Handle outside click.
        if ((row + 1) > mData->mGameGlobals.mRows) {
          std::cout << "Clicked outside" << std::endl;
        } else {
          // Handle inside click.
          std::cout << "Col: " << col + 1 << std::endl;
          std::cout << "Row: " << row + 1 << std::endl;
          int idx = (row * mData->mGameGlobals.mCols) + col;
          std::cout << "Position: " << idx << std::endl;
          mBoard.Init(col, row);
        }
      }
    }
  }
};

void GameScreen::Update(){};

void GameScreen::Draw() {
  mData->mWindow.clear();
  if (mBoard.mBoard->size() < 1) {
    DrawAllTiles("tile_hidden");
  } else {
      DrawRevealedBoard();
  }
  mData->mWindow.display();
};

int GameScreen::GetIndex(int col, int row) {
  return (row * mData->mGameGlobals.mCols) + col;
}

void GameScreen::DrawAllTiles(std::string texture) {
  for (int col = 0; col < mData->mGameGlobals.mCols; col++) {
    for (int row = 0; row < mData->mGameGlobals.mRows; row++) {
      sf::Sprite tile_hidden;
      tile_hidden.setTexture(mData->mAssetManager.GetTexture(texture));
      tile_hidden.setPosition(col * SQUARE, row * SQUARE);
      mData->mWindow.draw(tile_hidden);
    }
  }
}

void GameScreen::DrawOnlyMine() {
  for (int col = 0; col < mData->mGameGlobals.mCols; col++) {
    for (int row = 0; row < mData->mGameGlobals.mRows; row++) {
      sf::Sprite tile_revealed;
      tile_revealed.setTexture(
          mData->mAssetManager.GetTexture("tile_revealed"));
      tile_revealed.setPosition(col * SQUARE, row * SQUARE);
      mData->mWindow.draw(tile_revealed);
      if (mBoard.mBoard->at(GetIndex(col, row)).mIsMine) {
        sf::Sprite mine;
        mine.setTexture(mData->mAssetManager.GetTexture("mine"));
        mine.setPosition(col * SQUARE, row * SQUARE);
        mData->mWindow.draw(mine);
      }
    }
  }
}

void GameScreen::DrawRevealedBoard() {
  for (int col = 0; col < mData->mGameGlobals.mCols; col++) {
    for (int row = 0; row < mData->mGameGlobals.mRows; row++) {
      sf::Sprite tile_revealed;
      tile_revealed.setTexture(
          mData->mAssetManager.GetTexture("tile_revealed"));
      tile_revealed.setPosition(col * SQUARE, row * SQUARE);
      mData->mWindow.draw(tile_revealed);
      if (mBoard.mBoard->at(GetIndex(col, row)).mIsMine) {
        sf::Sprite mine;
        mine.setTexture(mData->mAssetManager.GetTexture("mine"));
        mine.setPosition(col * SQUARE, row * SQUARE);
        mData->mWindow.draw(mine);
      } else if (mBoard.mBoard->at(GetIndex(col, row)).mNeighborBombs != 0) {
        sf::Sprite number;
        switch (mBoard.mBoard->at(GetIndex(col, row)).mNeighborBombs) {
        case 1:
          number.setTexture(mData->mAssetManager.GetTexture("number_1"));
          break;
        case 2:
          number.setTexture(mData->mAssetManager.GetTexture("number_2"));
          break;
        case 3:
          number.setTexture(mData->mAssetManager.GetTexture("number_3"));
          break;
        case 4:
          number.setTexture(mData->mAssetManager.GetTexture("number_4"));
          break;
        case 5:
          number.setTexture(mData->mAssetManager.GetTexture("number_5"));
          break;
        case 6:
          number.setTexture(mData->mAssetManager.GetTexture("number_6"));
          break;
        case 7:
          number.setTexture(mData->mAssetManager.GetTexture("number_7"));
          break;
        case 8:
          number.setTexture(mData->mAssetManager.GetTexture("number_8"));
          break;
        }
        number.setPosition(col * SQUARE, row * SQUARE);
        mData->mWindow.draw(number);
      }
    }
  }
}

void GameScreen::SetAllSprites() {
  mDebug.setTexture(mData->mAssetManager.GetTexture("debug"));
  mDigits.setTexture(mData->mAssetManager.GetTexture("digits"));
  mFaceHappy.setTexture(mData->mAssetManager.GetTexture("face_happy"));
  mFaceLose.setTexture(mData->mAssetManager.GetTexture("face_lose"));
  mFaceWin.setTexture(mData->mAssetManager.GetTexture("face_win"));
  mFlag.setTexture(mData->mAssetManager.GetTexture("flag"));
  mLeaderboard.setTexture(mData->mAssetManager.GetTexture("leaderboard"));
  mMine.setTexture(mData->mAssetManager.GetTexture("mine"));
  mNumber1.setTexture(mData->mAssetManager.GetTexture("number_1"));
  mNumber2.setTexture(mData->mAssetManager.GetTexture("number_2"));
  mNumber3.setTexture(mData->mAssetManager.GetTexture("number_3"));
  mNumber4.setTexture(mData->mAssetManager.GetTexture("number_4"));
  mNumber5.setTexture(mData->mAssetManager.GetTexture("number_5"));
  mNumber6.setTexture(mData->mAssetManager.GetTexture("number_6"));
  mNumber7.setTexture(mData->mAssetManager.GetTexture("number_7"));
  mNumber8.setTexture(mData->mAssetManager.GetTexture("number_8"));
  mPause.setTexture(mData->mAssetManager.GetTexture("pause"));
  mPlay.setTexture(mData->mAssetManager.GetTexture("play"));
  mTileHidden.setTexture(mData->mAssetManager.GetTexture("tile_hidden"));
  mTileRevealed.setTexture(mData->mAssetManager.GetTexture("tile_revealed"));
};

void GameScreen::Pause(){};
void GameScreen::Resume(){};
