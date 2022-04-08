#include "screen.hpp"

using namespace std;

void PrintInMiddle(WINDOW *win, string str, int y) {
    mvwaddstr(win, y, (getmaxx(win) - str.length()) / 2, str.c_str());
}

void PrintPrompt(WINDOW *&win, string prompt, int lines, int y, int x) {
    if (x == -1) x = (COLS - prompt.length()) / 2;

    win = newwin(lines, COLS, y, 0);

    mvwaddstr(win, 0, x, prompt.c_str());

    wrefresh(win);
}

void EmptyWin(WINDOW *win) {
    wbkgd(win, COLOR_PAIR(0));
    wclear(win);
    wrefresh(win);
}

void RemoveWin(WINDOW *win) {
    wbkgd(win, COLOR_PAIR(0));
    wclear(win);
    wrefresh(win);
    delwin(win);
}