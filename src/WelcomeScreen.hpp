#include "Game.hpp"
#include "SFML/Graphics/Text.hpp"
#include "State.hpp"
#include "Constants.hpp"
#include <vector>

class WelcomeScreen : public State {
public:
  WelcomeScreen(GameDataRef gameDataRef);

  void Init() override;
  void Init(ExtrasRef extrasRef) override;

  void ProcessEvent() override;
  void Update() override;
  void Draw() override;

  void Pause() override;
  void Resume() override;

private:
  GameDataRef mData;
  std::string mPlayerName;
  sf::Text mH1;
  sf::Text mH2;
  sf::Text mPname;
};
