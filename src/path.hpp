#pragma once

#include "global.hpp"
#include "curses.h"

#include "test.hpp"

bool FindPath(Card **board, int boardHeight, int boardWidth, Pos *init, Pos *path);
void DrawPath(Pos *path);

bool CheckLineX(int x1, int x2, Card **board, int y);
bool CheckLineY(int y1, int y2, Card **board, int x);

bool CheckI(Pos p1, Pos p2, Card **board);

bool CheckZVertical(Pos pMinY, Pos pMaxY, Card **board, Pos* &path);
bool CheckZHorizontal(Pos pMinX, Pos pMaxX, Card **board, Pos* &path);
bool CheckZ(Pos p1, Pos p2, Card **board, Pos* &path);

bool CheckU_Up(Pos pMinX, Pos pMaxX, Card **board, int height, Pos* &path);
bool CheckU_Down(Pos pMinX, Pos pMaxX, Card **board, int height, Pos* &path);
bool CheckU_Left(Pos p1, Pos p2, Card **board, int width, Pos* &path);
bool CheckU_Right(Pos p1, Pos p2, Card **board, Pos* &path);
bool CheckU(Pos p1, Pos p2, Card **board, int height, int width, Pos* &path);

bool Hint(Card **board, int height, int width, Pos* &path);
bool CheckPaths(Pos p1, Pos p2, Card **board, int height, int width, Pos* &path);