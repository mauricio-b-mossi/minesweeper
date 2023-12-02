#include <iostream>
#include <fstream>
#include <string>

#include "Game.hpp"
#include "Constants.hpp"
#include "Constants.hpp"
#include "Welcome.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

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

    mData->mWindow.create(sf::VideoMode(SQUARE * cols, SQUARE * rows + 100), TITLE);

    mData->mStateManager.PushState(StateRef(new Welcome(mData)), false);

    Run();
}

void Game::Run(){
    while(mData->mWindow.isOpen()){
        mData->mStateManager.ProcessStateChanges();
        mData->mStateManager.GetCurrentState()->ProcessEvent();
        mData->mStateManager.GetCurrentState()->Update();
        mData->mStateManager.GetCurrentState()->Draw();
    }
}
