//
// Created by yauhen and marija on 25/09/2021.
//
#include "mazes.h"
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <SFML/System/Vector2.hpp>

char** allocateMemoryForMaze(int maxCol, int maxRow)
{
    char** a = new char*[maxRow];
    for(int i = 0; i < maxRow; ++i)
    {
        a[i] = new char[maxCol];
        //printf("Allocated");
    }

    for (int y = 0; y < maxRow; ++y)
    {
        for (int x = 0; x < maxCol; ++x)
        {
            a[y][x] = 32;
        }
    }

    for (int y = 0; y < maxRow; ++y)
    {
        a[y][0] = 120;
        a[y][maxCol - 1] = 120;
    }

    for (int x = 0; x < maxCol; ++x)
    {
        a[0][x] = 120;
        a[maxRow - 1][x] = 120;
    }

    return a;
}

int** allocateMemoryForMazeDistance(int maxCol, int maxRow)
{
    int** maze = new int*[maxRow];
    for(int i = 0; i < maxRow; ++i)
    {
        maze[i] = new int[maxCol];
        //printf("Allocated");
    }

    for (int y = 0; y < maxRow; ++y)
    {
        for (int x = 0; x < maxCol; ++x)
        {
            maze[y][x] = 0;
        }
    }

    return maze;
}

bool** allocateMemoryForMazeVisited(int maxCol, int maxRow)
{
    bool ** maze = new bool*[maxRow];
    for(int i = 0; i < maxRow; ++i)
    {
        maze[i] = new bool[maxCol];
        //printf("Allocated");
    }

    for (int y = 0; y < maxRow; ++y)
    {
        for (int x = 0; x < maxCol; ++x)
        {
            maze[y][x] = false;
        }
    }

    return maze;
}

void addMaze(char** memory, std::vector<sf::Vector2i> barrier)
{
    sf::Vector2i barrierTile;

    for (int i = 1; i < barrier.size(); ++i)
    {
        barrierTile = barrier[i];
        memory[barrierTile.y][barrierTile.x] = 120;
    }
}

void deleteMemoryForMaze(char **p, int maxCol)
{
    for(int i = 0; i < maxCol; ++i)
    {
        delete[] p[i];  //deletes an inner array of integer;
    }
    delete[] p;
}

void deleteMemoryForDistance(int **p, int maxCol)
{
    for(int i = 0; i < maxCol; ++i)
    {
        delete[] p[i];  //deletes an inner array of integer;
    }

    delete[] p;
}

void deleteMemoryForVisited(bool **p, int maxCol)
{
    for(int i = 0; i < maxCol; ++i)
    {
        delete[] p[i];  //deletes an inner array of integer;
    }

    delete[] p;
}
