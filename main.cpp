#include <cstdio>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "TileMap.h"
#include "Queue.h"
#include "List.h"
#include "mazes.h"
#include <fstream>
#include "StaticStack.h"

#define tileSize 32
#define byteSize 8
#define modeWidth 800
#define modeHeight 800

int FromTwoDToOneD (int x, int y, int );
sf::Vector2i FromOneDToTwoD (int , int);

int main()
{

    TileMap map;
    List<sf::Vector2i>* stack = new List<sf::Vector2i>;
    std::vector<sf::Vector2i> barrierList;
    sf::Vector2i maxSize;

    std::ifstream infile("Maze.txt");
    int x, y;
    while (infile >> x >> y)
        barrierList.emplace_back(sf::Vector2i(x,y));

    maxSize = barrierList[0];

    Queue queue = CreateQueue(maxSize.x * 2 + maxSize.y * 2);

    // create the window
    sf::RenderWindow window(sf::VideoMode(maxSize.y*tileSize + 2 , maxSize.x*tileSize + 2), "Project 4");
    sf::View view;
    char **maze = allocateMemoryForMaze(maxSize.y, maxSize.x);
    int **distance = allocateMemoryForMazeDistance(maxSize.y, maxSize.x);
    bool **visited = allocateMemoryForMazeVisited(maxSize.y, maxSize.x);

    addMaze(maze, barrierList);

    bool isStartSet = false;
    bool isFinishSet = false;

    Stack staticStack = CreateStack(maxSize.x * maxSize.y - barrierList.size());

    sf::Vector2i StartPosition;
    sf::Vector2i FinishPosition;
    sf::Vector2i northNeighbour;
    sf::Vector2i eastNeighbour;
    sf::Vector2i southNeighbour;
    sf::Vector2i westNeighbour;

    int i_StartPosition;
    int i_FinishPosition;

    int positionOneDimension;

    int level[maxSize.x * maxSize.y];  // 1D list of tiles

    for (int y = 0; y < maxSize.x; ++y)
    {
        for (int x = 0; x < maxSize.y; ++x)
        {
            positionOneDimension = FromTwoDToOneD(x, y, maxSize.y); // calculate position of 2D array in 1D

            if (maze[y][x] == 32)   // if empty. void I guess
            {
                level[positionOneDimension] = 0;   // place grass. very nice grass
            }
            else if (maze[y][x] == 120)
            {
                level[positionOneDimension] = 3;   // place stone. Big stone!
            }
            else if (maze[y][x] == 109 || maze[y][x] == 99) // c and m?
            {
                level[positionOneDimension] = 2;    // Planting trees == good for the environment
            }
            else
            {
                level[positionOneDimension] = 4;    // not in use
            }
        }
    }



    sf::Vector2i MousePosition;     // use it to store mouse position
    sf::Vector2i tilePosition;      // store coordinates of the tile pressed on

    while (window.isOpen())     // game loop
    {
        sf::Event event{};      // even queue!

        while (window.pollEvent(event))
        {

            if(event.type == sf::Event::Closed)
                window.close();

            //else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            //else if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            else if (event.type == event.MouseButtonReleased)
            {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

                // convert it to world coordinates
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                printf("\n\nx: %i, y %i here\n\n", worldPos.x/tileSize, worldPos.y/tileSize); // printing

                MousePosition = sf::Mouse::getPosition(window);     // get mouse position in relationship to the window

                tilePosition.x = MousePosition.x/tileSize;
                tilePosition.y = MousePosition.y/tileSize;

                if (maze[tilePosition.y][tilePosition.x] == 120)
                {
                    printf("Position (%i, %i) is not allowed", tilePosition.x, tilePosition.y);
                }
                else if(tilePosition.x > maxSize.x || tilePosition.x < 0 || tilePosition.y > maxSize.y || tilePosition.y < 0)
                {
                    printf("position is outside of the scope!");
                }
                else if (!isStartSet)
                {
                    isStartSet = true;

                    StartPosition.x = tilePosition.x;
                    StartPosition.y = tilePosition.y;

                    i_StartPosition = tilePosition.x + (tilePosition.y * maxSize.y);
                    level[i_StartPosition] = 2;   // place three in the tile

                    x = i_StartPosition % maxSize.y;
                    y = i_StartPosition / maxSize.y;

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
                popValue = FromOneDToTwoD(position, maxSize.y);
                if(popValue == FinishPosition)
                {
                    DisplayQueue(queue);
                    ReachedFinish = true;
                    printf("found!");

                    for (int y = 0; y < maxSize.x; ++y)
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
                        int north = FromTwoDToOneD(southNeighbour.x, southNeighbour.y, maxSize.y);
                        Enqueue(north, queue);
                    }
                    northNeighbour.y = popValue.y - 1;
                    northNeighbour.x = popValue.x;

                    if (!visited[northNeighbour.y][northNeighbour.x]
                    && maze[northNeighbour.y][northNeighbour.x] != 120)
                    {
                        visited[northNeighbour.y][northNeighbour.x] = true;
                        distance[northNeighbour.y][northNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int south = FromTwoDToOneD(northNeighbour.x, northNeighbour.y, maxSize.y);
                        Enqueue(south, queue);
                    }
                    eastNeighbour.y = popValue.y;
                    eastNeighbour.x = popValue.x + 1;

                    if (!visited[eastNeighbour.y][eastNeighbour.x]
                    && maze[eastNeighbour.y][eastNeighbour.x] != 120)
                    {
                        visited[eastNeighbour.y][eastNeighbour.x] = true;
                        distance[eastNeighbour.y][eastNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int east = FromTwoDToOneD(eastNeighbour.x, eastNeighbour.y, maxSize.y);
                        Enqueue(east, queue);
                    }

                    westNeighbour.y = popValue.y;
                    westNeighbour.x = popValue.x - 1;

                    if (!visited[westNeighbour.y][westNeighbour.x]
                    && maze[westNeighbour.y][westNeighbour.x]  != 120)
                    {
                        visited[westNeighbour.y][westNeighbour.x] = true;
                        distance[westNeighbour.y][westNeighbour.x] = distance[popValue.y][popValue.x] + 1;
                        int west = FromTwoDToOneD(westNeighbour.x, westNeighbour.y, maxSize.y);
                        Enqueue(west, queue);
                    }
                }
            }

            if (IsEmptyQueue(queue))
            {
                std::cout << "No path exists from (" << StartPosition.x << ", "<< StartPosition.y << ") to (" << FinishPosition.x << ", "<< StartPosition.y << ")" << std::endl;
                DisplayQueue(queue);

                while (queue->size != 0)
                {
                    Dequeue(queue);
                }

                // reset
                isFinishSet = false;
                isStartSet = false;

                distance = allocateMemoryForMazeDistance(maxSize.y, maxSize.x);
                visited = allocateMemoryForMazeVisited(maxSize.y, maxSize.x);

                maze[FinishPosition.y][FinishPosition.x] = 120;

                maze[StartPosition.y + 1][StartPosition.x] = 120;
                maze[StartPosition.y - 1][StartPosition.x] = 120;
                maze[StartPosition.y][StartPosition.x + 1] = 120;
                maze[StartPosition.y][StartPosition.x - 1] = 120;

                maze[StartPosition.y + 1][StartPosition.x + 1] = 120;
                maze[StartPosition.y - 1][StartPosition.x - 1] = 120;
                maze[StartPosition.y + 1][StartPosition.x - 1] = 120;
                maze[StartPosition.y - 1][StartPosition.x + 1] = 120;

                maze[FinishPosition.y + 1][FinishPosition.x] = 120;
                maze[FinishPosition.y - 1][FinishPosition.x] = 120;
                maze[FinishPosition.y][FinishPosition.x + 1] = 120;
                maze[FinishPosition.y][FinishPosition.x - 1] = 120;

                maze[FinishPosition.y + 1][FinishPosition.x + 1] = 120;
                maze[FinishPosition.y - 1][FinishPosition.x - 1] = 120;
                maze[FinishPosition.y + 1][FinishPosition.x - 1] = 120;
                maze[FinishPosition.y - 1][FinishPosition.x + 1] = 120;

            }else{ // slightly confused, but somehow works
                while (queue->size != 0)Dequeue(queue);
                stack->Push(&FinishPosition);
                sf::Vector2i* nextPosition = stack->GetLast();
                sf::Vector2i* test;
                maze[FinishPosition.y][FinishPosition.x] = 120;
                maze[StartPosition.y + 1][StartPosition.x] = 120;
                maze[StartPosition.y - 1][StartPosition.x] = 120;
                maze[StartPosition.y][StartPosition.x + 1] = 120;
                maze[StartPosition.y][StartPosition.x - 1] = 120;
                maze[StartPosition.y + 1][StartPosition.x + 1] = 120;
                maze[StartPosition.y - 1][StartPosition.x - 1] = 120;
                maze[StartPosition.y + 1][StartPosition.x - 1] = 120;
                maze[StartPosition.y - 1][StartPosition.x + 1] = 120;
                maze[FinishPosition.y + 1][FinishPosition.x] = 120;
                maze[FinishPosition.y - 1][FinishPosition.x] = 120;
                maze[FinishPosition.y][FinishPosition.x + 1] = 120;
                maze[FinishPosition.y][FinishPosition.x - 1] = 120;
                maze[FinishPosition.y + 1][FinishPosition.x + 1] = 120;
                maze[FinishPosition.y - 1][FinishPosition.x - 1] = 120;
                maze[FinishPosition.y + 1][FinishPosition.x - 1] = 120;
                maze[FinishPosition.y - 1][FinishPosition.x + 1] = 120;
                while(true){if (*nextPosition == StartPosition)break;   // don't worry about this. I am an engineer
                    southNeighbour.y = nextPosition->y + 1;             // be happy
                    southNeighbour.x = nextPosition->x;
                    northNeighbour.y = nextPosition->y - 1;
                    northNeighbour.x = nextPosition->x;
                    eastNeighbour.y = nextPosition->y;
                    eastNeighbour.x = nextPosition->x + 1;
                    westNeighbour.y = nextPosition->y;
                    westNeighbour.x = nextPosition->x - 1;
                    if (visited[southNeighbour.y][southNeighbour.x]
                    && distance[southNeighbour.y][southNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {stack->Push(&southNeighbour);
                        int south = FromTwoDToOneD((int)southNeighbour.x, (int)southNeighbour.y, maxSize.y);
                        PushStack(south, staticStack);
                        *nextPosition = southNeighbour;
                        test = stack->GetLast();
                        printf("%i, %i", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y, maxSize.y)] = 2;
                        maze[test->y][test->x] = 120;
                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                        maze[test->y + 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x - 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;
                    }else if (visited[northNeighbour.y][northNeighbour.x]
                    && distance[northNeighbour.y][northNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {stack->Push(&northNeighbour);
                        int north = FromTwoDToOneD((int)northNeighbour.x, (int)northNeighbour.y, maxSize.y);
                        PushStack(north, staticStack);
                        *nextPosition = northNeighbour;
                        test = stack->GetLast();
                        printf("%i, %i", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y, maxSize.y)] = 2;
                        maze[test->y][test->x] = 120;
                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                        maze[test->y + 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x - 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;}
                    else if (visited[eastNeighbour.y][eastNeighbour.x]
                    && distance[eastNeighbour.y][eastNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1)
                    {int east = FromTwoDToOneD((int)eastNeighbour.x, (int)eastNeighbour.y, maxSize.y);
                        PushStack(east, staticStack);
                        stack->Push(&eastNeighbour);
                        *nextPosition = eastNeighbour;
                        test = stack->GetLast();
                        printf("%i, %i", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y, maxSize.y)] = 2;
                        maze[test->y][test->x] = 120;
                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                        maze[test->y + 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x - 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;}
                    else if (visited[westNeighbour.y][westNeighbour.x]
                    && distance[westNeighbour.y][westNeighbour.x] == distance[nextPosition->y][nextPosition->x] - 1){
                        int west = FromTwoDToOneD((int)westNeighbour.x, (int)westNeighbour.y, maxSize.y);
                        PushStack(west, staticStack);
                        stack->Push(&westNeighbour);
                        *nextPosition = westNeighbour;
                        test = stack->GetLast();
                        printf("\n\n%i, %i\n\n", test->x, test->y);
                        level[FromTwoDToOneD(test->x, test->y, maxSize.y)] = 2;
                        maze[test->y][test->x] = 120;
                        maze[test ->y + 1][test->x] = 120;
                        maze[test->y - 1][test->x] = 120;
                        maze[test ->y][test->x + 1] = 120;
                        maze[test->y][test->x - 1] = 120;
                        maze[test->y + 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x - 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;
                        maze[test->y - 1][test->x + 1] = 120;}}
                sf::Vector2i linePosition;
                int linePositionInt;
                // get position from the stack and add tree
                while(true){linePositionInt = PopStack(staticStack);linePosition = FromOneDToTwoD(linePositionInt, maxSize.y);level[linePositionInt] = 2;if (staticStack->topOfStack == 0)break;}
                maze[StartPosition.y][StartPosition.x] = 120;
                maze[FinishPosition.y][FinishPosition.x] = 120;
                isFinishSet = false;
                isStartSet = false;
                distance = allocateMemoryForMazeDistance(maxSize.y, maxSize.x);
                visited = allocateMemoryForMazeVisited(maxSize.y, maxSize.x);}}
        if (!map.load("tiles.png", sf::Vector2u(32, 32), level, maxSize.y, maxSize.x))  // if ?
            return -1;      //  it returns -1
        window.clear();
        window.draw(map);
        window.display();}
    while (queue->size != 0)    // cleans up
        Dequeue(queue);
    infile.close(); // infile close
    deleteMemoryForMaze(maze, maxSize.y);   // delete something, not good?
    deleteMemoryForDistance(distance, maxSize.y);
    deleteMemoryForVisited(visited, maxSize.y);
    return 0;   // finished
}

int FromTwoDToOneD (int x, int y, int maxrow)   // don't do drugs
{
    int i = x + ( y * maxrow); // i
    return i;
}

sf::Vector2i FromOneDToTwoD (int i, int maxrow)     // math
{
    sf::Vector2i temp; // temp
    temp.x = i % maxrow;
    temp.y = i / maxrow;

    return temp;
}