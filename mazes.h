//
// Created by yauhe on 25/09/2021.
//

#ifndef PROJECT_4_MAZES_H
#define PROJECT_4_MAZES_H

#include <vector>
#include <SFML/System/Vector2.hpp>

#define MAX_ROWS 12
#define MAX_COLUMNS 30

char** allocateMemoryForMaze(int , int );
int** allocateMemoryForMazeDistance(int , int );
bool** allocateMemoryForMazeVisited(int , int );
void deleteMemoryForMaze(char **);
void deleteMemoryForDistance(int **);
void deleteMemoryForVisited(bool **);
void addMaze(char**, std::vector<sf::Vector2i>);

#endif //PROJECT_4_MAZES_H
