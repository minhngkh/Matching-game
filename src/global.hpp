#pragma once

#include "curses.h"
#include <string>

struct Pos {
    int y, x;
};

struct Card {
    char val;
    std::string status = "none";
    WINDOW *win;
};