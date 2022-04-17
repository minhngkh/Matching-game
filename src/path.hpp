#pragma once

#include "global.hpp"
#include "curses.h"
#include "string"

bool CheckPaths(Pos p1, Pos p2, Card **board, int height, int width, Pos* &path, int &pathLen);
bool FindHint(Card **board, int height, int width, Pos* &path, int &pathLen);

#define DR_UP 1
#define DR_DOWN -1
#define DR_LEFT 2
#define DR_RIGHT -2

void DrawPath(Card **board, int boardHeight, int boardWidth, Pos *path, int &pathLen);