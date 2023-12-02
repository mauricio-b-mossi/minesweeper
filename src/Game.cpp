#include "Game.hpp"
#include <fstream>
#include <string>
#include "Constants.hpp"
#include <iostream>

// Perform initialization, load fonts, read files, etc.
Game::Game(){
    // Read Data set window size.
    std::ifstream f(CONFIG_PATH);
    std::string temp;
    std::getline(f, temp);
    int cols = std::stoi(temp);
    std::getline(f, temp);
    int rows = std::stoi(temp);
    std::getline(f, temp);
    int mines = std::stoi(temp);
    f.close();
    std::cout << "Cols: " << cols << " Rows: " << rows << " Mines: " << mines << std::endl;
}
