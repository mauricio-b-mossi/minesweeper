#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>
#include <unordered_map>

class AssetManager {
    public:
        void LoadFont(std::string name, std::string filePath);
        void LoadTexture(std::string name, std::string filePath);
        sf::Font& GetFont(std::string name);
        sf::Texture& GetTexture(std::string name);


// Private that way I can load defaults if something crashes.
private:
  std::unordered_map<std::string, sf::Texture> _textures;
  std::unordered_map<std::string, sf::Font> _fonts;
};
