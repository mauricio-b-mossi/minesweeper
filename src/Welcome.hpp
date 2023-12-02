#include "Game.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "State.hpp"

class Welcome : public State {
public:
  Welcome(GameDataRef gameDataRef);

  void Init() override;

  void ProcessEvent() override;
  void Update() override;
  void Draw() override;

  void Pause() override;
  void Resume() override;

private:
  GameDataRef mData;
  std::string mPlayer;
};
