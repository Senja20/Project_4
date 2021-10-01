//
// Created by yauhe on 25/09/2021.
//

#include "mazes.h"


#include <cstdlib>
#include <cstdio>

char MazeExample[MAX_ROWS][MAX_COLUMNS] = {"xxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                                           "x                           x",
                                           "x     x                     x",
                                           "x   xxx                     x",
                                           "x                           x",
                                           "x                           x",
                                           "x                           x",
                                           "x                           x",
                                           "x      x                    x",
                                           "x     x                     x",
                                           "x     xxx                   x",
                                           "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};

char** allocateMemoryForMaze()
{
    char** a = new char*[MAX_ROWS];
    for(int i = 0; i < MAX_ROWS; ++i)
    {
        a[i] = new char[MAX_COLUMNS];
        printf("Allocated");
    }

    return a;
}

int** allocateMemoryForMazeDistance()
{
    int** maze = new int*[MAX_ROWS];
    for(int i = 0; i < MAX_ROWS; ++i)
    {
        maze[i] = new int[MAX_COLUMNS];
        //printf("Allocated");
    }

    for (int y = 0; y < MAX_ROWS; ++y)
    {
        for (int x = 0; x < MAX_COLUMNS; ++x)
        {
            maze[y][x] = 0;
        }
    }

    return maze;
}

bool** allocateMemoryForMazeVisited()
{
    bool ** maze = new bool*[MAX_ROWS];
    for(int i = 0; i < MAX_ROWS; ++i)
    {
        maze[i] = new bool[MAX_COLUMNS];
        //printf("Allocated");
    }

    for (int y = 0; y < MAX_ROWS; ++y)
    {
        for (int x = 0; x < MAX_COLUMNS; ++x)
        {
            maze[y][x] = false;
        }
    }

    return maze;
}

void addMaze(char** memory)
{
    for (int y = 0; y < MAX_ROWS; ++y)
    {
        for (int x = 0; x < MAX_COLUMNS; ++x)
        {
            memory[y][x] = MazeExample[y][x];
        }
    }
}

void deleteMemoryForMaze(char **p)
{
    for(int i = 0; i < MAX_COLUMNS; ++i)
    {
        delete[] p[i];  //deletes an inner array of integer;
    }
    delete[] p;
}

void deleteMemoryForDistance(int **p)
{
    for(int i = 0; i < MAX_COLUMNS; ++i){
        delete[] p[i];  //deletes an inner array of integer;
    }

    delete[] p;
}

void deleteMemoryForVisited(bool **p)
{
    for(int i = 0; i < MAX_COLUMNS; ++i){
        delete[] p[i];  //deletes an inner array of integer;
    }

    delete[] p;
}
