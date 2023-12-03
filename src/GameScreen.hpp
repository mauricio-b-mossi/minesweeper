#include "Game.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "State.hpp"
#include "Constants.hpp"
#include <vector>
#include "Board.hpp"

class GameScreen : public State {
public:
  GameScreen(GameDataRef gameDataRef);

  void Init() override;
  void Init(ExtrasRef extrasRef) override;

  void ProcessEvent() override;
  void Update() override;
  void Draw() override;

  void Pause() override;
  void Resume() override;

private:
  GameDataRef mData;
  bool mGameStarted = false;
  sf::Sprite mDefaultTile;
  sf::Text mPname;
  std::string mPlayerName;
  Board mBoard;
};
