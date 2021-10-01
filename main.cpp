#include <cstdio>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TileMap.h"
#include "Queue.h"
#include "mazes.h"
#include <malloc.h>

#define tileSize 32
#define byteSize 8

int FromTwoDToOneD (int x, int y);
sf::Vector2i FromOneDToTwoD (int i);

int main()
{
    Queue queue = CreateQueue(100);

    char **maze = allocateMemoryForMaze();
    int **distance = allocateMemoryForMazeDistance();
    bool **visited = allocateMemoryForMazeVisited();

    const int max_col = _msize(maze[0]);
    const int max_row = _msize(maze)/byteSize;

    addMaze(maze);

    bool isStartSet = false;
    bool isFinishSet = false;

    sf::Vector2i StartPosition;
    sf::Vector2i FinishPosition;
    sf::Vector2i northNeighbour;
    sf::Vector2i eastNeighbour;
    sf::Vector2i southNeighbour;
    sf::Vector2i westNeighbour;

    int i_StartPosition;
    int i_FinishPosition;

    int positionOneDimension;

    int level[max_row * max_col];  // 1D list of tiles

    for (int y = 0; y < max_row; ++y)
    {
        for (int x = 0; x < max_col; ++x)
        {
            positionOneDimension = FromTwoDToOneD(x, y); // calculate position of 2D array in 1D

            if (maze[y][x] == 32)   // if empty
            {
                level[positionOneDimension] = 0;   // place grass
            }
            else if (maze[y][x] == 120) // if obstacle
            {
                level[positionOneDimension] = 3;   // place stone
                visited[y][x] = true;
            }
            else if (maze[y][x] == 109 || maze[y][x] == 99) // chess or mouse
            {
                level[positionOneDimension] = 2;    // place threes
            }
            else
            {
                level[positionOneDimension] = 4;   // place black tile
            }
        }
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(1536 , 864), "Tilman");
    TileMap map;

    sf::Vector2i MousePosition;     // use it to store mouse position
    sf::Vector2i tilePosition;      // store coordinates of the tile pressed on

    while (window.isOpen())     // game loop
    {
        sf::Event event{};      // even queue

        while (window.pollEvent(event))
        {

            if(event.type == sf::Event::Closed)
                window.close();

            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                MousePosition = sf::Mouse::getPosition(window);     // get mouse position in relationship to the window

                tilePosition.x = MousePosition.x/tileSize;
                tilePosition.y = MousePosition.y/tileSize;

                if (!isStartSet)
                {
                    isStartSet = true;

                    StartPosition.x = tilePosition.x;
                    StartPosition.y = tilePosition.y;

                    i_StartPosition = tilePosition.x + (tilePosition.y * max_col);
                    level[i_StartPosition] = 2;   // place three in the tile

                    int x = i_StartPosition % max_col;

                    int y = i_StartPosition / max_col;

                    std::cout << "x:" << x << "y: " << y << std::endl;

                    maze[StartPosition.y][StartPosition.x] = 109;
                }

                else if (!isFinishSet)
                {
                    isFinishSet = true;

                    FinishPosition.x = tilePosition.x;
                    FinishPosition.y = tilePosition.y;


                    i_FinishPosition = tilePosition.x + (tilePosition.y * max_col);

                    level[i_FinishPosition] = 2;   // place three in the tile

                    maze[FinishPosition.y][FinishPosition.x] = 99;
                }
            }
        }

        if (isStartSet && isFinishSet)
        {

            sf::Vector2i popValue;
            int position;

            distance[StartPosition.y][StartPosition.x] = 0;
            visited[StartPosition.y][StartPosition.x] = true;

            Enqueue(i_StartPosition, queue);


            bool ReachedFinish = false;

            while ((!IsEmptyQueue(queue)) && !ReachedFinish)
            {
                position = FrontOfQueue(queue);
                Dequeue(queue);

                popValue = FromOneDToTwoD(position);

                if(popValue == FinishPosition)
                {
                    ReachedFinish = true;
                    printf("found!");

                    for (int y = 0; y < max_row; ++y)
                    {
                        printf("\n");
                        for (int x = 0; x < max_col; ++x)
                        {
                            printf("%i", distance[y][x]);
                        }
                    }

                }

                // cheeseFound = the next element is the square with the cheese;
                // what does that mean!?

                if (!ReachedFinish)
                {
                    northNeighbour.y = popValue.y + 1;
                    northNeighbour.x = popValue.x;

                    if (!visited[northNeighbour.y][northNeighbour.x])
                    {

                        visited[northNeighbour.y][northNeighbour.x] = true;
                        distance[northNeighbour.y][northNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int north = FromTwoDToOneD(northNeighbour.x, northNeighbour.y);
                        Enqueue(north, queue);

                    }

                    southNeighbour.y = popValue.y - 1;
                    southNeighbour.x = popValue.x;

                    if (!visited[southNeighbour.y][southNeighbour.x])
                    {
                        visited[southNeighbour.y][southNeighbour.x] = true;
                        distance[southNeighbour.y][southNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int south = FromTwoDToOneD(southNeighbour.x, southNeighbour.y);
                        Enqueue(south, queue);
                    }

                    eastNeighbour.y = popValue.y;
                    eastNeighbour.x = popValue.x + 1;

                    if (!visited[eastNeighbour.y][eastNeighbour.x])
                    {
                        visited[eastNeighbour.y][eastNeighbour.x] = true;
                        distance[eastNeighbour.y][eastNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int east = FromTwoDToOneD(eastNeighbour.x, eastNeighbour.y);
                        Enqueue(east, queue);
                    }

                    westNeighbour.y = popValue.y;
                    westNeighbour.x = popValue.x - 1;

                    if (!visited[westNeighbour.y][westNeighbour.x])
                    {
                        visited[westNeighbour.y][westNeighbour.x] = true;
                        distance[westNeighbour.y][westNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int west = FromTwoDToOneD(westNeighbour.x, westNeighbour.y);
                        Enqueue(west, queue);
                    }


                }
            }
        }

        if (!map.load("tiles.png", sf::Vector2u(32, 32), level, max_col, max_row))
            return -1;

        window.clear();
        window.draw(map);
        window.display();
    }

    deleteMemoryForMaze(maze);
    deleteMemoryForDistance(distance);
    deleteMemoryForVisited(visited);

    return 0;
}

int FromTwoDToOneD (int x, int y)
{
    int i = x + ( y * MAX_COLUMNS);
    return i;
}

sf::Vector2i FromOneDToTwoD (int i)
{
    sf::Vector2i temp;

    temp.x = i % MAX_COLUMNS;
    temp.y = i / MAX_COLUMNS;

    return temp;
}