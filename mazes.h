//
// Created by yauhe on 25/09/2021.
//

#ifndef PROJECT_4_MAZES_H
#define PROJECT_4_MAZES_H

#include <vector>
#include <SFML/System/Vector2.hpp>

char** allocateMemoryForMaze(int , int );
int** allocateMemoryForMazeDistance(int , int );
bool** allocateMemoryForMazeVisited(int , int );
void deleteMemoryForMaze(char **, int);
void deleteMemoryForDistance(int **, int);
void deleteMemoryForVisited(bool **, int);
void addMaze(char**, std::vector<sf::Vector2i>);

#endif //PROJECT_4_MAZES_H
