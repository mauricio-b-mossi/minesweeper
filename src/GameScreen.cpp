#include "GameScreen.hpp"
#include "Constants.hpp"
#include "Game.hpp"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
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
#include <string>
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

  mYposDigit = mYactionRow + 16;

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
    if (mData->mGameGlobals.mDebug) {
      if (event.type == sf::Event::KeyPressed) {
        if (!mStopWatch.mIsFreezed && mBoard.mBoard->size() > 0) {
          Solve();
          mStopWatch.Freeze();
        }
      }
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
              event.mouseButton.y < mYactionRow + BUTTON) {
            std::cout << "Clicked Action Row" << std::endl;
            if (event.mouseButton.x > mXposFace &&
                event.mouseButton.x < mXposFace + BUTTON && !IsPaused()) {
              if (mStopWatch.mIsFreezed) {
                mStopWatch.mIsFreezed = false;
              }
              mStopWatch.Reset();
              mBoard.Erase();
              mBoard.mHasLost = false;
            } else if (event.mouseButton.x > mXposPlayPause &&
                       event.mouseButton.x < mXposPlayPause + BUTTON &&
                       mBoard.mBoard->size() > 0) {
              mIsPlaying = !mIsPlaying;
              mStopWatch.Toggle();
            } else if (event.mouseButton.x > mXposLeaderboard &&
                       event.mouseButton.x < mXposLeaderboard + BUTTON) {
            } else if (event.mouseButton.x > mXposDebug &&
                       event.mouseButton.x < mXposDebug + BUTTON &&
                       !IsPaused()) {
              mIsDebugging = !mIsDebugging;
            }
          }
        } else {

          // Handle inside click.
          std::cout << "Col: " << col + 1 << std::endl;
          std::cout << "Row: " << row + 1 << std::endl;
          int idx = (row * mData->mGameGlobals.mCols) + col;
          std::cout << "Position: " << idx << std::endl;

          if (!mBoard.mHasLost && !IsPaused()) {
            if (mBoard.mBoard->size() < 1) {
              std::cout << "INITIALIZING BOARD" << std::endl;
              mBoard.Init(col, row);
              mStopWatch.Start();
              mIsPlaying = true;
            } else {
              if (mBoard.Open(col, row)) {
                if (mBoard.HasWon()) {
                  // Navigate.
                  mStopWatch.Freeze();
                }
              } else
                mStopWatch.Freeze();
            }
          }
        }
      } else if (event.mouseButton.button == sf::Mouse::Right) {

        std::cout << "Right clicked" << std::endl;

        int col = event.mouseButton.x / 32;
        int row = event.mouseButton.y / 32;

        if (row < mData->mGameGlobals.mRows && mBoard.mBoard->size() > 0 &&
            !mBoard.mHasLost && !IsPaused()) {
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
    if (mBoard.HasWon()) {
      DrawBoardState(true);
    } else if (!mIsPlaying && mBoard.mBoard->size() > 0) {
      DrawAllTiles("tile_revealed");
    } else if (mIsDebugging) {
      DrawOnlyMine();
    } else {
      DrawBoardState(false);
    }
  }
  DrawControls(mData->mGameGlobals.mCustom);
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

void GameScreen::DrawBoardState(bool flags) {
  for (int col = 0; col < mData->mGameGlobals.mCols; col++) {
    for (int row = 0; row < mData->mGameGlobals.mRows; row++) {

      if (mBoard.mBoard->at(GetIndex(col, row)).mIsOpen) {
        sf::Sprite tile_revealed;

        tile_revealed.setTexture(
            mData->mAssetManager.GetTexture("tile_revealed"));
        tile_revealed.setPosition(col * SQUARE, row * SQUARE);
        mData->mWindow.draw(tile_revealed);

        if (mBoard.mBoard->at(GetIndex(col, row)).mIsMine) {
          sf::Sprite flagOrMine;
          if (flags) {
            flagOrMine.setTexture(mData->mAssetManager.GetTexture("flag"));
          } else
            flagOrMine.setTexture(mData->mAssetManager.GetTexture("mine"));
          flagOrMine.setPosition(col * SQUARE, row * SQUARE);
          mData->mWindow.draw(flagOrMine);

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

void GameScreen::DrawControls(bool custom) {
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

  if (custom) {
    if (mIsDebugging)
      debug.setTexture(mData->mAssetManager.GetTexture("open-eyes"));
    else
      debug.setTexture(mData->mAssetManager.GetTexture("close-eyes"));
  } else {
    debug.setTexture(mData->mAssetManager.GetTexture("debug"));
  }

  face.setPosition(mXposFace, mYposFace);

  play_pause.setPosition(mXposPlayPause, mYposFace);

  debug.setPosition(mXposDebug, mYposDebug);

  DrawCounter();

  DrawStopwatch(mData->mGameGlobals.mCols * 32 - 97, 2,
                mStopWatch.GetMinutes());
  DrawStopwatch(mData->mGameGlobals.mCols * 32 - 54, 2,
                mStopWatch.GetSeconds());

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

bool GameScreen::IsPaused() { return mBoard.mBoard->size() > 0 && !mIsPlaying; }

void GameScreen::DrawCounter() {
  sf::Sprite digit;
  digit.setTexture(mData->mAssetManager.GetTexture("digits"));

  std::string digits = std::to_string(mBoard.mMines - (*mBoard.mFlaggedCells));
  int offset = 33;
  if (digits[0] == '-')
    offset = 12;
  for (int i = 0; i < digits.length(); i++) {
    sf::Sprite digit;
    switch (digits[i]) {
    case '0':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '1':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 1, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '2':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 2, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '3':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 3, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '4':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 4, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '5':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 5, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '6':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 6, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '7':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 7, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '8':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 8, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '9':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 9, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    case '-':
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
      digit.setPosition(offset + (i * DIGIT), mYposDigit);
      break;
    }
    mData->mWindow.draw(digit);
  }
};

void GameScreen::DrawStopwatch(int offset, int padding, int number) {

  sf::Sprite digit;
  digit.setTexture(mData->mAssetManager.GetTexture("digits"));

  std::string digits = std::to_string(number);

  int paddingZeros = 0;

  if (digits.length() < padding) {
    paddingZeros = padding - digits.length();
  }

  // size_t has some weird behavior as a type.
  for (int i = (0 - paddingZeros); i < (int)(digits.length()); i++) {

    sf::Sprite digit;

    if (i < 0) {
      digit.setTexture(mData->mAssetManager.GetTexture("digits"));
      digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
      digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
    } else {
      switch (digits[i]) {
      case '0':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '1':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 1, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '2':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 2, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '3':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 3, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '4':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 4, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '5':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 5, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '6':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 6, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '7':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 7, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '8':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 8, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '9':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 9, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      case '-':
        digit.setTexture(mData->mAssetManager.GetTexture("digits"));
        digit.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
        digit.setPosition(offset + ((i + paddingZeros) * DIGIT), mYposDigit);
        break;
      }
    }

    mData->mWindow.draw(digit);
  }
};

void GameScreen::Solve() {
  for (auto &c : (*mBoard.mBoard)) {
    c.Open();
  }
}
