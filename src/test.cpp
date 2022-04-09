#include "test.hpp"

bool test() {
    WINDOW *win;

    PrintPrompt(win, "Remove? (0/1)", 1, LINES - 1);

    int ch;

    while(true) {
        ch = getch();
        switch(ch) {
            case '0': 
                RemoveWin(win);
                return false;
            case '1': 
                RemoveWin(win);
                return true;
            default: 
                break;
        }
    }
}