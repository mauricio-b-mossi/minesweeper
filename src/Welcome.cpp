#include <iostream>
#include "Welcome.hpp"
#include "Game.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

Welcome::Welcome(GameDataRef gameDataRef) : mData(gameDataRef){};

void Welcome::Init() {}

void Welcome::ProcessEvent() {
  sf::Event ev;

  while (mData->mWindow.pollEvent(ev)) {
    if (ev.type == sf::Event::Closed) {
      mData->mWindow.close();
    } else if(ev.type == sf::Event::KeyPressed){
        std::cout << ev.key.code << std::endl;
    }
  }
}

void Welcome::Update() {
}

void Welcome::Draw() {
}

void Welcome::Pause() {}
void Welcome::Resume() {}
