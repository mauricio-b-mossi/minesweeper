#include "AssetManager.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <iostream>

void AssetManager::LoadFont(std::string name, std::string filePath) {
  sf::Font font;
  if (font.loadFromFile(filePath)) {
    _fonts[name] = font;
  } else {
    std::cout << "Font Load Error: " << name << " could not be loaded from "
              << filePath << std::endl;
  }
}

void AssetManager::LoadTexture(std::string name, std::string filePath) {
  sf::Texture texture;
  if (texture.loadFromFile(filePath)) {
    _textures[name] = texture;
  } else {
    std::cout << "Texture Load Error: " << name << " could not be loaded from "
              << filePath << std::endl;
  }
}

sf::Texture &AssetManager::GetTexture(std::string name) {
  return _textures.at(name);
}

sf::Font &AssetManager::GetFont(std::string name) { return _fonts.at(name); }
