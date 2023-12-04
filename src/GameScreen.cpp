#include "GameScreen.hpp"
#include "Constants.hpp"
#include "Game.hpp"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "State.hpp"
#include <ctype.h>
#include <iostream>
#include <memory>
#include <vector>

GameScreen::GameScreen(GameDataRef gameDataRef)
    : mData(gameDataRef),
      mBoard(mData->mGameGlobals.mCols, mData->mGameGlobals.mRows,
             mData->mGameGlobals.mMines) {

  mXposDebug = (mData->mGameGlobals.mCols * 32) - 304;
  mYposDebug = 32 * (mData->mGameGlobals.mRows + 0.5);

  mXposPlayPause = (mData->mGameGlobals.mCols * 32) - 240;
  mYposPlayPause = 32 * (mData->mGameGlobals.mRows + 0.5);

  mXposLeaderboard = (mData->mGameGlobals.mCols * 32) - 176;
  mYposLeaderboard = 32 * (mData->mGameGlobals.mRows + 0.5);

  mXposFace = (mData->mGameGlobals.mCols / 2.f * 32) - 32;
  mYposFace = 32 * (mData->mGameGlobals.mRows + 0.5);

  mYactionRow = 32 * (mData->mGameGlobals.mRows + 0.5);

  SetConstantSprites();
};

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
          std::cout << "Clicked Outside Board" << std::endl;
          // Check if within action row;
          if (event.mouseButton.y > mYactionRow &&
              event.mouseButton.y < mYactionRow + 64) {
            std::cout << "Clicked Action Row" << std::endl;
            if (event.mouseButton.x > mXposFace &&
                event.mouseButton.x < mXposFace + 64) {
              mBoard.Erase();
              mBoard.mHasLost = false;
            } else if (event.mouseButton.x > mXposPlayPause &&
                       event.mouseButton.x < mXposPlayPause + 64) {
              mIsPlaying = !mIsPlaying;
            } else if (event.mouseButton.x > mXposLeaderboard &&
                       event.mouseButton.x < mXposLeaderboard + 64) {
            } else if (event.mouseButton.x > mXposDebug &&
                       event.mouseButton.x < mXposDebug + 64) {
              mIsDebugging = !mIsDebugging;
            }
          }
        } else {

          // Handle inside click.
          std::cout << "Col: " << col + 1 << std::endl;
          std::cout << "Row: " << row + 1 << std::endl;
          int idx = (row * mData->mGameGlobals.mCols) + col;
          std::cout << "Position: " << idx << std::endl;

          if (!mBoard.mHasLost) {
            if (mBoard.mBoard->size() < 1) {
              std::cout << "INITIALIZING BOARD" << std::endl;
              mBoard.Init(col, row);
              mIsPlaying = true;
            } else {
              mBoard.Open(col, row);
            }
          }
        }
      } else if (event.mouseButton.button == sf::Mouse::Right) {

        std::cout << "Right clicked" << std::endl;

        int col = event.mouseButton.x / 32;
        int row = event.mouseButton.y / 32;

        if (row < mData->mGameGlobals.mRows && mBoard.mBoard->size() > 0 &&
            !mBoard.mHasLost) {
          mBoard.Flag(col, row);
        }
      }
    }
  }
}

void GameScreen::Update(){};

void GameScreen::Draw() {
  mData->mWindow.clear();
  if (mBoard.mBoard->size() < 1) {
    DrawAllTiles("tile_hidden");
  } else {
    if (mIsDebugging) {
      DrawOnlyMine();
    } else {
      DrawBoardState();
    }
  }
  DrawControls();
  mData->mWindow.display();
};

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

void GameScreen::DrawBoardState() {
  for (int col = 0; col < mData->mGameGlobals.mCols; col++) {
    for (int row = 0; row < mData->mGameGlobals.mRows; row++) {

      if (mBoard.mBoard->at(GetIndex(col, row)).mIsOpen) {
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
      } else {
        sf::Sprite tile_hidden;
        tile_hidden.setTexture(mData->mAssetManager.GetTexture("tile_hidden"));
        tile_hidden.setPosition(col * SQUARE, row * SQUARE);
        mData->mWindow.draw(tile_hidden);

        if (mBoard.mBoard->at(GetIndex(col, row)).mIsMarked) {
          sf::Sprite flag;
          flag.setTexture(mData->mAssetManager.GetTexture("flag"));
          flag.setPosition(col * SQUARE, row * SQUARE);
          mData->mWindow.draw(flag);
        }
      }
    }
  }
}

void GameScreen::DrawOnlyMine() {
  for (int col = 0; col < mData->mGameGlobals.mCols; col++) {
    for (int row = 0; row < mData->mGameGlobals.mRows; row++) {
      sf::Sprite tile_hidden;
      tile_hidden.setTexture(mData->mAssetManager.GetTexture("tile_hidden"));
      tile_hidden.setPosition(col * SQUARE, row * SQUARE);
      mData->mWindow.draw(tile_hidden);
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

void GameScreen::DrawControls() {
  sf::Sprite face;
  sf::Sprite play_pause;
  sf::Sprite debug;

  if (mBoard.HasWon())
    face.setTexture(mData->mAssetManager.GetTexture("face_win"));
  else if (mBoard.mHasLost)
    face.setTexture(mData->mAssetManager.GetTexture("face_lose"));
  else
    face.setTexture(mData->mAssetManager.GetTexture("face_happy"));

  if (mIsPlaying)
    play_pause.setTexture(mData->mAssetManager.GetTexture("play"));
  else
    play_pause.setTexture(mData->mAssetManager.GetTexture("pause"));

  if (mIsDebugging)
    debug.setTexture(mData->mAssetManager.GetTexture("open-eyes"));
  else
    debug.setTexture(mData->mAssetManager.GetTexture("close-eyes"));

  face.setPosition(mXposFace, mYposFace);

  play_pause.setPosition(mXposPlayPause, mYposFace);

  debug.setPosition(mXposDebug, mYposDebug);

  mData->mWindow.draw(face);
  mData->mWindow.draw(play_pause);
  mData->mWindow.draw(debug);
  mData->mWindow.draw(mLeaderboard);
}

void GameScreen::Pause(){};
void GameScreen::Resume(){};

int GameScreen::GetIndex(int col, int row) {
  return (row * mData->mGameGlobals.mCols) + col;
}

void GameScreen::SetConstantSprites() {
  mLeaderboard.setTexture(mData->mAssetManager.GetTexture("leaderboard"));
  mLeaderboard.setPosition(mXposLeaderboard, mYposLeaderboard);
};
