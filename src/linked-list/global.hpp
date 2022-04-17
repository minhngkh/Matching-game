#pragma once

#include "curses.h"

#define STATUS_NONE 0
#define STATUS_HIGHLIGHTED 1
#define STATUS_SELECTED 2
#define STATUS_REMOVED 3

struct Box {
    WINDOW *cover;
    WINDOW *core;
};
struct Pos {
    int y, x;
};

struct Card {
    char val;
    int status = STATUS_NONE;
    Box win;
};