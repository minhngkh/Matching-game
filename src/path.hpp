#pragma once

#include "global.hpp"
#include "ncurses.h"

#include "test.hpp"

bool FindPath(Card **board, int boardHeight, int boardWidth, Pos *init, Pos *path);
void DrawPath(Pos *path);


bool CheckLineX(int x1, int x2, char **board, int y);
bool CheckLineY(int y1, int y2, char**board, int x);
bool CheckI(Pos p1, Pos p2, char **board);

bool CheckZVertical(Pos pMinY, Pos pMaxY, char** board);
bool CheckZHorizontal(Pos pMinX, Pos pMaxX, char** board);
bool CheckZ(Pos p1, Pos p2, char** board);

bool CheckU_Up(Pos pMinX, Pos pMaxX, char** board, int height);
bool CheckU_Down(Pos pMinX, Pos pMaxX, char** board, int height);
bool CheckU_Left(Pos p1, Pos p2, char** board, int width);
bool CheckU_Right(Pos p1, Pos p2, char** board);
bool CheckU(Pos p1, Pos p2, char** board, int height, int width);

bool CheckPaths(Pos p1, Pos p2, char **board, int height, int width);