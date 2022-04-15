#pragma once

#include "global.hpp"
#include "curses.h"
#include <string>
#include <fstream>

#define MENU_PADDING 3
#define MENU_SPACE 0
int ChooseMenu(std::string *menu, int options);

void PrintInMiddle(WINDOW *win, std::string str);
void PrintPrompt(WINDOW *&win, std::string prompt, int lines = 1, int y = LINES / 2, int x = -1);

void EmptyWin(WINDOW *win);
void RemoveWin(WINDOW *win);

void DisplayBackground(WINDOW *&win);