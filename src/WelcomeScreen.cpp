#include "WelcomeScreen.hpp"
#include "Constants.hpp"
#include "Game.hpp"
#include "GameScreen.hpp"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "State.hpp"
#include <ctype.h>
#include <iostream>
#include <memory>
#include <vector>

std::string FormatNameInput(std::string name) {
  if (name.length() < 10)
    return name + "|";
  else
    return name;
};

WelcomeScreen::WelcomeScreen(GameDataRef gameDataRef) : mData(gameDataRef){};

void WelcomeScreen::Init() {
  mH1.setFont(mData->mAssetManager.GetFont("font"));
  mH2.setFont(mData->mAssetManager.GetFont("font"));
  mPname.setFont(mData->mAssetManager.GetFont("font"));

  mH1.setCharacterSize(H1);
  mH2.setCharacterSize(H2);
  mPname.setCharacterSize(P);

  mH1.setString(WELCOME_H1);
  mH2.setString(WELCOME_H2);
  mPname.setString(FormatNameInput(mPlayerName));

  mH1.setStyle(sf::Text::Bold | sf::Text::Underlined);
  mH2.setStyle(sf::Text::Bold);
  mPname.setStyle(sf::Text::Bold);

  mH1.setPosition(
      (mData->mWindow.getSize().x / 2.f - mH1.getLocalBounds().width / 2.f),
      (mData->mWindow.getSize().y / 2.f - mH1.getLocalBounds().height / 2.f -
       150));
  mH2.setPosition(
      (mData->mWindow.getSize().x / 2.f - mH2.getLocalBounds().width / 2.f),
      (mData->mWindow.getSize().y / 2.f - mH2.getLocalBounds().height / 2.f -
       75));
  mPname.setPosition(
      (mData->mWindow.getSize().x / 2.f - mPname.getLocalBounds().width / 2.f),
      (mData->mWindow.getSize().y / 2.f - mPname.getLocalBounds().height / 2.f -
       45));
};

void WelcomeScreen::Init(ExtrasRef extrasRef) { Init(); };

void WelcomeScreen::ProcessEvent() {
  sf::Event event;

  while (mData->mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mData->mWindow.close();
    } else {
      if (event.type == sf::Event::TextEntered) {

        if (event.text.unicode > 64 && event.text.unicode < 91 ||
            event.text.unicode > 96 && event.text.unicode < 123) {
          std::cout << static_cast<char>(event.text.unicode) << ": "
                    << event.text.unicode << std::endl;
          if (mPlayerName.length() < 10) {
            if (mPlayerName.length() == 0) {
              mPlayerName.push_back(toupper(event.text.unicode));
            } else
              mPlayerName.push_back(tolower(event.text.unicode));
          }
        } else if (event.text.unicode == 8) {
          std::cout << "BackSpace" << std::endl;
          if (mPlayerName.length() > 0)
            mPlayerName.pop_back();
        } else if (event.text.unicode == 13) {
          std::cout << "CR" << std::endl;
          if (mPlayerName.length() > 0) {
            ExtrasRef extras(new std::vector<std::string>);
            extras->emplace_back(mPlayerName);
            mData->mStateManager.PushState(StateRef(new GameScreen(mData)),
                                           true, std::move(extras));
          }
        }
      }
    }
  }
};

void WelcomeScreen::Update() {
  mPname.setString(FormatNameInput(mPlayerName));
  mPname.setPosition(
      (mData->mWindow.getSize().x / 2.f - mPname.getLocalBounds().width / 2.f),
      (mData->mWindow.getSize().y / 2.f - mPname.getLocalBounds().height / 2.f -
       45));
};

void WelcomeScreen::Draw() {
  mData->mWindow.clear();
  mData->mWindow.draw(mH1);
  mData->mWindow.draw(mH2);
  mData->mWindow.draw(mPname);
  mData->mWindow.display();
};

void WelcomeScreen::Pause(){};
void WelcomeScreen::Resume(){};
