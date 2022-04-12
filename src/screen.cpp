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

void DisplayBackground(WINDOW *win) {
    ifstream ifs("resources/poke.art");

    if (!ifs) return;

    string line;
    int height, width;
    height = width = 0;

    // get the size of the ascii art
    while(getline(ifs, line)) {
        if (line.length() > width) width = line.length();
        ++height;
    }

    // reset to the beginning
    ifs.clear();
    ifs.seekg(0);

    win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

    // print the art out
    int i = 0;
    while(getline(ifs, line)) {
        mvwaddstr(win, i, 0, line.c_str());
        ++i;
    }

    wrefresh(win);
}