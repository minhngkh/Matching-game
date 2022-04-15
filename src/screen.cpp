#include "screen.hpp"

using namespace std;

void InteractMenu(Box *menuWins, int options, int hightlight) {
    for (int i = 0; i < options; i++) {
        if (hightlight == i) {
            wbkgd(menuWins[i].content, COLOR_PAIR(1));
        } else {
            wbkgd(menuWins[i].content, COLOR_PAIR(0));
        }
        wrefresh(menuWins[i].border);
        wrefresh(menuWins[i].content);
    }
}

int ChooseMenu(string *menu, int options) {
    int highlight = 0;
    int choice = 0;
    int ch;

    int menuWidth = 0;
    for (int i = 0; i < options; i++) {
        if (menu[i].length() > menuWidth) menuWidth = menu[i].length();
    }

    menuWidth += MENU_PADDING;
    Box *menuWins = new Box[options];
    for (int i = 0; i < options; i++) {
        menuWins[i].border = newwin(3, menuWidth + 2, (LINES - options * 3) / 2 + i * 3, (COLS - menuWidth - 2) / 2 );
        menuWins[i].content = derwin(menuWins[i].border, 1, menuWidth, 1, 1);
        box(menuWins[i].border, 0, 0);
        mvwaddstr(menuWins[i].content, 0, (menuWidth - menu[i].length()) / 2, menu[i].c_str());
        wrefresh(menuWins[i].border);
    }
    InteractMenu(menuWins, options, highlight);
    
    while(true) {
        ch = getch();
        switch(ch) {
            case 'w':
            case 'W':
            case KEY_UP:
                if(highlight > 0) --highlight;
                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                if(highlight < options - 1) ++highlight;
                break;

            case '\r':
            case '\n':
            case KEY_ENTER:
                choice = highlight;
                return choice;
                
            default:
                break;
        }

        InteractMenu(menuWins, options, highlight);
    }

    return -1;
}

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

void DisplayBackground(WINDOW *&win) {
    ifstream ifs("resources/poke.art");

    if (!ifs) return;

    string line;
    int height, width;
    height = width = 0;

    // get the size of the ascii art
    // name conflict so i have to specify std here
    while(getline(ifs, line)) {
        if (int(line.length()) > width) width = line.length();
        ++height;
    }

    // reset to the beginning
    ifs.clear();
    ifs.seekg(0);

    win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    wattron(win, A_DIM);

    // print the art out
    int i = 0;
    while(getline(ifs, line)) {
        mvwaddstr(win, i, 0, line.c_str());
        ++i;
    }

    wrefresh(win);
}