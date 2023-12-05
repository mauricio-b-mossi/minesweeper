#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char **args) {
  if (argc > 1) {
    std::string arg{args[1]};
    if (arg == "-c") {
      std::cout << "Running CUSTOM Game" << std::endl;
      Game game{true, false};
    } else if (arg == "-d") {
      std::cout << "Running DEBUG Game" << std::endl;
      Game game{false, true};
    } else if (arg == "-cd") {
      std::cout << "Running CUSTOM DEBUG Game" << std::endl;
      Game game{true, true};
    }
  } else {
    std::cout << "Running STANDARD Game" << std::endl;
    Game game;
  }
}
