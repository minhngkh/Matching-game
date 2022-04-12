#pragma once

#include "global.hpp"
#include "ncurses.h"

#include "test.hpp"

bool FindPath(Card **board, int boardHeight, int boardWidth, Pos *init, Pos *path);
void DrawPath(Pos *path);


bool CheckLineX(int x1, int x2, Card **board, int y);
bool CheckLineY(int y1, int y2, Card**board, int x);
bool CheckI(Pos p1, Pos p2, Card **board);

bool CheckZVertical(Pos pMinY, Pos pMaxY, Card** board);
bool CheckZHorizontal(Pos pMinX, Pos pMaxX, Card** board);
bool CheckZ(Pos p1, Pos p2, Card** board);

bool CheckU_Up(Pos pMinX, Pos pMaxX, Card** board, int height);
bool CheckU_Down(Pos pMinX, Pos pMaxX, Card** board, int height);
bool CheckU_Left(Pos p1, Pos p2, Card** board, int width);
bool CheckU_Right(Pos p1, Pos p2, Card** board);
bool CheckU(Pos p1, Pos p2, Card** board, int height, int width);

bool CheckPaths(Pos p1, Pos p2, Card **board, int height, int width);