#pragma once

#include "global.hpp"
#include "ncurses.h"

#include "test.hpp"

bool FindPath(Card **board, int boardHeight, int boardWidth, Pos *init, Pos *path);
void DrawPath(Pos *path);