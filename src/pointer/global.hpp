#pragma once

#include "curses.h"

#define CARD_WIDTH 5
#define CARD_HEIGHT 3
#define CARD_SPACE 1


struct Box {
    WINDOW *cover;
    WINDOW *core;
};
struct Pos {
    int y, x;
};

// Different status of a card
#define STATUS_NONE 0
#define STATUS_HIGHLIGHTED 1
#define STATUS_SELECTED 2
#define STATUS_REMOVED 3
struct Card {
    char val;
    int status = STATUS_NONE;
    Box win;
};