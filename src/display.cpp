#include "display.hpp"

using namespace std;

void InteractMenu(Box *menuWins, int options, int hightlight) {
    for (int i = 0; i < options; i++) {
        if (hightlight == i) {
            wbkgd(menuWins[i].core, COLOR_PAIR(1));
        } else {
            wbkgd(menuWins[i].core, COLOR_PAIR(0));
        }
        wrefresh(menuWins[i].cover);
        wrefresh(menuWins[i].core);
    }
}

int ChooseMenu(string *menu, int options) {
    clear();
    refresh();

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
        menuWins[i].cover = newwin(3, menuWidth + 2, (LINES - options * 3) / 2 + i * 3, (COLS - menuWidth - 2) / 2 );
        menuWins[i].core = derwin(menuWins[i].cover, 1, menuWidth, 1, 1);
        box(menuWins[i].cover, 0, 0);
        mvwaddstr(menuWins[i].core, 0, (menuWidth - menu[i].length()) / 2, menu[i].c_str());
        wrefresh(menuWins[i].core);
        wrefresh(menuWins[i].cover);
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

void DisplayArt(WINDOW *&win, string art) {
    ifstream ifs(art);

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

int PlayGame(int height, int width, int mode) {
    // Generate board
    Card **board;
    GenerateBoard(board, height, width);
    //GenerateTest(board, height, width);

    WINDOW *background;
    DisplayArt(background, BACKGROUND);
    DisplayBoard(board, height, width);

    // Prompt before start
    WINDOW *promptWin;
    PrintPrompt(promptWin, "Press any key to continue", 1, LINES - 1);

    getch();

    RemoveWin(promptWin);

    // Play
    int pairsRemoved = 0;
    int totalPairs = height * width / 2;

    while (pairsRemoved < totalPairs) {
        clear();
        refresh();
        touchwin(background);
        wrefresh(background);
        RefreshBoard(board, height, width);
        Pos *selectedPos = new Pos[2];

        int gameState = GetInput(board, height, width, selectedPos);
        if (gameState != NORMAL) {
            clear();
            RemoveWin(background);
            refresh();
            return gameState;
        }

        Pos *path;
        int pathLen;

        if (CheckPaths(selectedPos[0], selectedPos[1], board, height, width, path, pathLen)) {
        // if (FindPath(board, height, width, selectedPos, path)) {
            DrawPath(board, height, width, path, pathLen);
            getch();
            //napms(1000); // Delay 1000ms
            RemovePair(board, selectedPos);
            ++pairsRemoved;
        } else {
            TogglePair(board, selectedPos);
        }
    }

    clear();
    RemoveWin(background);
    refresh();
    
    return FINISHED;
}

void DisplayEndScreen(int mode) {
    WINDOW *prompt;
    if (mode == SURRENDER) {
        DisplayArt(prompt, LOSE_PROMPT);
    } 
    // win
    DisplayArt(prompt, WIN_PROMPT);

    WINDOW *inputWin;

    const int SPACE_INPUT = 10;
    string out = "Enter your name:";
    string in;
    
    int startX = (COLS - out.length() - SPACE_INPUT) / 2;
    PrintPrompt(inputWin, out, 1, LINES - 2, startX);

    echo();
    nocbreak();
    wrefresh(inputWin);
    curs_set(1);

    move(LINES - 2, startX + out.length() + 1);

    int ch = getch();
    while ( ch != '\n' )
    {
        in.push_back( ch );
        ch = getch();
    }
    

    noecho();
    raw();
    curs_set(0);

    RemoveWin(inputWin);

    mvaddstr(0,0,in.c_str());

    getch();
}