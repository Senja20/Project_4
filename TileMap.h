#ifndef PROJECT_4_TILEMAP_H
#define PROJECT_4_TILEMAP_H

#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string &tileset, sf::Vector2u tileSize, const int *tiles, unsigned int width, unsigned int height);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::VertexArray vertices;
    sf::Texture tileSet;

};


#endif //PROJECT_4_TILEMAP_H
