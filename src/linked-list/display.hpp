#pragma once

#include "global.hpp"
#include "board.hpp"
#include "path.hpp"
#include "curses.h"
#include "extra.hpp"

#include <string>
#include <fstream>

#define MENU_PADDING 1
#define MENU_SPACE 0

int ChooseMenu(std::string *menu, int options);

void PrintInMiddle(WINDOW *win, std::string str, int y);
void PrintPrompt(WINDOW *&win, std::string prompt, int lines = 1, int y = LINES / 2, int x = -1);

void EmptyWin(WINDOW *win);
void RemoveWin(WINDOW *win);

#define BACKGROUND "resources/poke.art"
#define WIN_PROMPT "resources/win.art"
#define LOSE_PROMPT "resources/lose.art"

void DisplayArt(WINDOW *&win, std::string art);

#define MODE_NORMAL 0
#define MODE_DIFFICULT 1

int PlayGame(int height, int width, int mode, int &timeFinished);

void DisplayEndScreen(int mode, int height, int width, int time = -1);

#define NUM_LEADERBOARD 5

void DisplayLeaderboard(int height, int width);