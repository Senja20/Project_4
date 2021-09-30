//
// Created by yauhe on 25/09/2021.
//

#ifndef PROJECT_4_MAZES_H
#define PROJECT_4_MAZES_H

#define MAX_ROWS 21
#define MAX_COLUMNS 44

char** allocateMemoryForMaze();
int** allocateMemoryForMazeDistance();
bool** allocateMemoryForMazeVisited();
void deleteMemoryForMaze(char **);
void deleteMemoryForDistance(int **);
void deleteMemoryForVisited(bool **);
void addMaze(char** );

#endif //PROJECT_4_MAZES_H
