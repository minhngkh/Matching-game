#pragma once

#include "curses.h"
#include <string>

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

struct Card {
    char val;
    std::string status = "none";
    Box win;
};