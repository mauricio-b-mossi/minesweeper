#include <iostream>
#include "Welcome.hpp"
#include "Game.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

Welcome::Welcome(GameDataRef gameDataRef) : mData(gameDataRef){};

void Welcome::Init() {
  circle.setRadius(100.f);
  circle.setFillColor(sf::Color::Blue);
  circle.setPosition(
      sf::Vector2f(mData->mWindow.getSize().x / 2.f - circle.getRadius(),
                   mData->mWindow.getSize().x / 2.f - circle.getRadius()));
}

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
  if (circle.getPosition().x < 0 ||
      circle.getPosition().x > mData->mWindow.getSize().x)
    dx *= -1;
  if (circle.getPosition().y < 0 ||
      circle.getPosition().y > mData->mWindow.getSize().y)
    dy *= -1;

  circle.move(sf::Vector2f(dx, dy));
}

void Welcome::Draw() {
  mData->mWindow.clear();
  mData->mWindow.draw(circle);
  mData->mWindow.display();
}

void Welcome::Pause() {}
void Welcome::Resume() {}
