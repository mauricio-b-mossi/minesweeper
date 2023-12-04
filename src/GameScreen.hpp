#include "Game.hpp"
#include "Board.hpp"
#include "Constants.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "State.hpp"
#include <vector>

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
  void SetConstantSprites();
  void DrawAllTiles(std::string texture);
  void DrawOnlyMine();
  void DrawRevealedBoard();
  void DrawControls();
  int GetIndex(int col, int row);

  enum ControlState { WON = 1, PAUSED = 2 };

private:
  GameDataRef mData;
  bool mGamePlaying = false;

  sf::Sprite mDebug;
  sf::Sprite mLeaderboard;
  std::string mPlayerName;

  Board mBoard;
};
