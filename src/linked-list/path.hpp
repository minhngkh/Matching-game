#pragma once

#include "global.hpp"
#include "curses.h"
#include "linked-list.hpp"
#include "string"

bool CheckPaths(Pos p1, Pos p2, List *board, int height, int width, Path &newPath);
bool FindHint(List *board, int height, int width, Path &newPath);

#define DR_UP 1
#define DR_DOWN -1
#define DR_LEFT 2
#define DR_RIGHT -2

void DrawPath(List *board, int boardHeight, int boardWidth, Path currPath);
