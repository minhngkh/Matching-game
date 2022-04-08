#pragma once

#include "global.hpp"
#include <ncurses.h>
#include <string>

void PrintInMiddle(WINDOW *win, std::string str);
void PrintPrompt(WINDOW *&win, std::string prompt, int lines = 1, int y = LINES / 2, int x = -1);
void EmptyWin(WINDOW *win);
void RemoveWin(WINDOW *win);