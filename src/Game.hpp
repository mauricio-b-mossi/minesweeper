#pragma once
#include "AssetManager.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "StateManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

struct GameData {
  // AssetManager and StateManager and StateGlobals.
public:
  StateManager mStateManager;
  AssetManager mAssetManager;
  sf::RenderWindow mWindow;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game {
public:
  Game();
  void Run();

private:
  GameDataRef mData = std::make_shared<GameData>();
};
