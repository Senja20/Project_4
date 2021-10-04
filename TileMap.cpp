#include "TileMap.h"

bool TileMap::load(const std::string& textureSet, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    int numberOfTiles = 0;
    int tilex, tiley;
    unsigned int i,j;
    sf::Vertex* square;

    if (!tileSet.loadFromFile(textureSet))
    {
        return false;
    }

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            numberOfTiles = tiles[i + j * width];

            tilex = numberOfTiles % ((tileSet.getSize().x) / tileSize.x);
            tiley = numberOfTiles / ((tileSet.getSize().x) / tileSize.x);

            square = &vertices[(i + j * width) * 4];

            square[0].position = sf::Vector2f(i * tileSize.x+ 2, j * tileSize.y+2);
            square[1].position = sf::Vector2f((i + 1) * tileSize.x+2, j * tileSize.y+2);
            square[2].position = sf::Vector2f((i + 1) * tileSize.x+2, (j + 1) * tileSize.y+2);
            square[3].position = sf::Vector2f(i * tileSize.x+2, (j + 1) * tileSize.y+2);

            square[0].texCoords = sf::Vector2f(tilex * tileSize.x, tiley * tileSize.y);
            square[1].texCoords = sf::Vector2f((tilex + 1) * tileSize.x, tiley * tileSize.y);
            square[2].texCoords = sf::Vector2f((tilex + 1) * tileSize.x, (tiley + 1) * tileSize.y);
            square[3].texCoords = sf::Vector2f(tilex * tileSize.x, (tiley + 1) * tileSize.y);
        }
    }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &tileSet;
    target.draw(vertices, states);
}
