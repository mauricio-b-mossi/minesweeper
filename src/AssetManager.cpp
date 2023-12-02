#include "AssetManager.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

void AssetManager::LoadFont(std::string name, std::string filePath) {
  sf::Font font;
  if (font.loadFromFile(filePath)) {
    _fonts[name] = font;
  }
}

void AssetManager::LoadTexture(std::string name, std::string filePath) {
  sf::Texture texture;
  if (texture.loadFromFile(filePath)) {
    _textures[name] = texture;
  }
}

sf::Texture& AssetManager::GetTexture(std::string name){
    return _textures.at(name);
}

sf::Font& AssetManager::GetFont(std::string name){
    return _fonts.at(name);
}
