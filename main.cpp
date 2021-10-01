#include <cstdio>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TileMap.h"
#include "Queue.h"
#include "List.h"
#include "mazes.h"
#include <malloc.h>
#include <fstream>

#define tileSize 32
#define byteSize 8

int FromTwoDToOneD (int x, int y);
sf::Vector2i FromOneDToTwoD (int i);

int main()
{
    Queue queue = CreateQueue(100);
    List<sf::Vector2i>* stack = new List<sf::Vector2i>;

    char **maze = allocateMemoryForMaze();
    int **distance = allocateMemoryForMazeDistance();
    bool **visited = allocateMemoryForMazeVisited();

    std::ifstream infile("Maze.txt");
    int x, y;

    std::vector<sf::Vector2i> barrierList;
    sf::Vector2i maxSize;

    while (infile >> x >> y)
    {
        barrierList.emplace_back(sf::Vector2i(x,y));
    }

    maxSize = barrierList[0];

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

    int level[MAX_ROWS * maxSize.y];  // 1D list of tiles

    for (int y = 0; y < MAX_ROWS; ++y)
    {
        for (int x = 0; x < maxSize.y; ++x)
        {
            positionOneDimension = FromTwoDToOneD(x, y); // calculate position of 2D array in 1D

            if (maze[y][x] == 32)   // if empty
            {
                level[positionOneDimension] = 0;   // place grass
            }
            else if (maze[y][x] == 120) // if obstacle
            {
                level[positionOneDimension] = 3;   // place stone
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

                    i_StartPosition = tilePosition.x + (tilePosition.y * maxSize.y);
                    level[i_StartPosition] = 2;   // place three in the tile

                    int x = i_StartPosition % maxSize.y;

                    int y = i_StartPosition / maxSize.y;

                    std::cout << "x:" << x << "y: " << y << std::endl;

                    maze[StartPosition.y][StartPosition.x] = 109;
                }

                else if (!isFinishSet)
                {
                    isFinishSet = true;

                    FinishPosition.x = tilePosition.x;
                    FinishPosition.y = tilePosition.y;

                    i_FinishPosition = tilePosition.x + (tilePosition.y * maxSize.y);

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

            while (!IsEmptyQueue(queue) && !ReachedFinish)
            {
                position = FrontOfQueue(queue);
                Dequeue(queue);

                popValue = FromOneDToTwoD(position);

                if(popValue == FinishPosition)
                {
                    ReachedFinish = true;
                    printf("found!");

                    for (int y = 0; y < MAX_ROWS; ++y)
                    {
                        printf("\n");
                        for (int x = 0; x < maxSize.y; ++x)
                        {
                            printf("%c", maze[y][x]);
                        }
                    }

                }

                // cheeseFound = the next element is the square with the cheese;
                // what does that mean!?

                if (!ReachedFinish)
                {
                    southNeighbour.y = popValue.y + 1;
                    southNeighbour.x = popValue.x;

                    if (!visited[southNeighbour.y][southNeighbour.x]
                    &&  maze[southNeighbour.y][southNeighbour.x] != 120)
                    {
                        visited[southNeighbour.y][southNeighbour.x] = true;
                        distance[southNeighbour.y][southNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int north = FromTwoDToOneD(southNeighbour.x, southNeighbour.y);
                        Enqueue(north, queue);
                    }

                    northNeighbour.y = popValue.y - 1;
                    northNeighbour.x = popValue.x;

                    if (!visited[northNeighbour.y][northNeighbour.x]
                    && maze[northNeighbour.y][northNeighbour.x] != 120)
                    {
                        visited[northNeighbour.y][northNeighbour.x] = true;
                        distance[northNeighbour.y][northNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int south = FromTwoDToOneD(northNeighbour.x, northNeighbour.y);
                        Enqueue(south, queue);
                    }

                    eastNeighbour.y = popValue.y;
                    eastNeighbour.x = popValue.x + 1;

                    if (!visited[eastNeighbour.y][eastNeighbour.x]
                    && maze[eastNeighbour.y][eastNeighbour.x] != 120)
                    {
                        visited[eastNeighbour.y][eastNeighbour.x] = true;
                        distance[eastNeighbour.y][eastNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int east = FromTwoDToOneD(eastNeighbour.x, eastNeighbour.y);
                        Enqueue(east, queue);
                    }

                    westNeighbour.y = popValue.y;
                    westNeighbour.x = popValue.x - 1;

                    if (!visited[westNeighbour.y][westNeighbour.x]
                    && maze[westNeighbour.y][westNeighbour.x]  != 120)
                    {
                        visited[westNeighbour.y][westNeighbour.x] = true;
                        distance[westNeighbour.y][westNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int west = FromTwoDToOneD(westNeighbour.x, westNeighbour.y);
                        Enqueue(west, queue);
                    }
                }
            }

            if (IsEmptyQueue(queue))
            {
                std::cout << "No path exists from (" << StartPosition.x << ", "<< StartPosition.y << ") to (" << FinishPosition.x << ", "<< StartPosition.y << ")" << std::endl;
            }

            else
            {


                while (queue->size != 0)
                {
                    Dequeue(queue);
                }

                stack->Push(&FinishPosition);
                sf::Vector2i* nextPosition = stack->GetLast();
                sf::Vector2i* test;

                while(true)
                {
                    if (*nextPosition == StartPosition)
                    {
                        break;
                    }

                    southNeighbour.y = nextPosition->y + 1;
                    southNeighbour.x = nextPosition->x;

                    northNeighbour.y = nextPosition->y - 1;
                    northNeighbour.x = nextPosition->x;

                    eastNeighbour.y = nextPosition->y;
                    eastNeighbour.x = nextPosition->x + 1;

                    westNeighbour.y = nextPosition->y;
                    westNeighbour.x = nextPosition->x - 1;

                    if(westNeighbour == *nextPosition)
                    {
                        nextPosition->x++;
                    }

                    if (visited[southNeighbour.y][southNeighbour.x]
                    && distance[southNeighbour.y][southNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {
                        stack->Push(&southNeighbour);
                        *nextPosition = southNeighbour;
                        test = stack->GetLast();
                        printf("%i, %i", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y)] = 2;
                        maze[test->y][test->x] = 120;

                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;


                    }

                    else if (visited[northNeighbour.y][northNeighbour.x]
                    && distance[northNeighbour.y][northNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {
                        stack->Push(&northNeighbour);
                        *nextPosition = northNeighbour;
                        test = stack->GetLast();
                        printf("%i, %i", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y)] = 2;
                        maze[test->y][test->x] = 120;

                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                    }

                    else if (visited[eastNeighbour.y][eastNeighbour.x]
                    && distance[eastNeighbour.y][eastNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {

                        stack->Push(&eastNeighbour);
                        *nextPosition = eastNeighbour;
                        test = stack->GetLast();
                        printf("%i, %i", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y)] = 2;
                        maze[test->y][test->x] = 120;

                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                    }

                    else if (visited[westNeighbour.y][westNeighbour.x]
                    && distance[westNeighbour.y][westNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {
                        stack->Push(&westNeighbour);
                        *nextPosition = westNeighbour;
                        test = stack->GetLast();
                        printf("\n\n%i, %i\n\n", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y)] = 2;
                        maze[test->y][test->x] = 120;

                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                    }
                }

                sf::Vector2i* linePosition;
                int linePositionInt;

                /*while(stack->GetLength() != 0)
                {
                    linePosition = stack->GetLast();
                    linePositionInt = FromTwoDToOneD(linePosition->x, linePosition->y);
                    level[linePositionInt] = 2;
                    stack->Pop();
                }*/

                maze[StartPosition.y][StartPosition.x] = 120;
                maze[FinishPosition.y][FinishPosition.x] = 120;

                maze[StartPosition.y + 1][StartPosition.x] = 120;
                maze[StartPosition.y - 1][StartPosition.x] = 120;
                maze[StartPosition.y][StartPosition.x + 1] = 120;
                maze[StartPosition.y][StartPosition.x - 1] = 120;


                // reset
                isFinishSet = false;
                isStartSet = false;

                distance = allocateMemoryForMazeDistance();
                visited = allocateMemoryForMazeVisited();
            }
        }

        if (!map.load("tiles.png", sf::Vector2u(32, 32), level, maxSize.y, MAX_ROWS))
            return -1;

        window.clear();
        window.draw(map);
        window.display();
    }

    while (queue->size != 0)
    {
        Dequeue(queue);
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