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

    // padding for both left and right side
    menuWidth += MENU_PADDING * 2;
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

int PlayGame(int height, int width, int mode, int &timeFinished) {
    // Generate board
    List *board;
    GenerateBoard(board, height, width);
    //GenerateTest(board, height, width);

    WINDOW *background;
    DisplayArt(background, BACKGROUND);
    DisplayBoard(board, height, width);

    // Prompt before start
    WINDOW *promptWin;
    PrintPrompt(promptWin, "Press any key to continue", 1, LINES - 2);

    getch();

    RemoveWin(promptWin);

    // Play
    Time startTime = GetCurrTime();
    
    int pairsRemoved = 0;
    int totalPairs = height * width / 2;

    while (pairsRemoved < totalPairs) {
        clear();
        refresh();
        touchwin(background);
        wrefresh(background);
        RefreshBoard(board, height);

        WINDOW *instructWin;
        PrintPrompt(instructWin, "8: endgame check  9: Help  0: Surrender", 1, 1);

        Pos *selectedPos = new Pos[2];
        Path path;

        int gameState = GetInput(board, height, width, selectedPos, path);

        if (gameState == ST_ASSISTED) {
            ToggleCard(GetNode(board, selectedPos[0])->data);
            ToggleCard(GetNode(board, selectedPos[1])->data);
            
            // wait for user to recognize the pair
            while (true) {
                char ch = getch();
                if (ch == '\r' || ch == '\n' || ch == KEY_ENTER) break;
            }

            CorrectSound();

            DrawPath(board, height, width, path);
            refresh();

            // delay 150 ms
            napms(150);

            // remove any character pressed when the delay happens
            flushinp();

            RemovePair(board, selectedPos);

            if (mode == MODE_DIFFICULT) SlideBoard(board, selectedPos);

            ++pairsRemoved;
            continue;
        }

        if (gameState == ST_RESET) {
            WINDOW *prompt;
            PrintPrompt(prompt, "Valid pair(s) existed. Press any key to continue", 1, LINES - 2);
            getch();
            RemoveWin(prompt);
            continue;
        }

        if (gameState == ST_NOPAIRS) {
            ErrorSound();

            WINDOW *prompt;
            PrintPrompt(prompt, "No valid pair left. Press any key to end the game", 1, LINES - 2);
            
            getch();
            clear();
            RemoveWin(background);
            RemoveWin(prompt);
            refresh();

            return ST_SURRENDER;
        }

        if (gameState != ST_NORMAL) {
            clear();
            RemoveWin(background);
            refresh();
            return gameState;
        }

        if (CheckPaths(selectedPos[0], selectedPos[1], board, height, width, path)) {
        //if (test()) {
            CorrectSound();
            
            DrawPath(board, height, width, path);
            refresh();
            // delay 150 ms
            napms(150);
            // remove any character pressed when the delay happens
            flushinp();

            RemovePair(board, selectedPos);

            if (mode == MODE_DIFFICULT) SlideBoard(board, selectedPos);

            ++pairsRemoved;
        } else {
            ErrorSound();
            
            TogglePair(board, selectedPos);
        }
    }

    clear();
    RemoveWin(background);
    refresh();

    timeFinished = ElapsedTime(GetCurrTime(), startTime);
    
    return ST_FINISHED;
}

void DisplayEndScreen(int mode, int height, int width, int time) {
    WINDOW *prompt;

    if (mode == ST_SURRENDER) {
        LoseSound();
        DisplayArt(prompt, LOSE_PROMPT);
        getch();

        return;
    } 
    
    // win
    WinSound();
    DisplayArt(prompt, WIN_PROMPT);

    WINDOW *timeWin;
    string timePrompt = "Finished in " + to_string(time) + " sec(s)";
    PrintPrompt(timeWin, timePrompt.c_str(), 1, LINES - 4);

    WINDOW *inputWin;

    const int SPACE_INPUT = 10;
    string out = "Enter your name:";

    // crate an input buffer
    char buffer[255];
    
    int startX = (COLS - out.length() - SPACE_INPUT) / 2;
    PrintPrompt(inputWin, out, 1, LINES - 2, startX);

    echo();
    cbreak();
    wrefresh(inputWin);
    curs_set(1);

    mvwgetstr(inputWin, 0, startX + out.length() + 1, buffer);
    
    noecho();
    raw();
    curs_set(0);

    RemoveWin(inputWin);
    RemoveWin(timeWin);

    // manually handle the buffer
    Stat data;
    strncpy(data.name, buffer, 10 - 1);
    data.name[9] = '\0';

    data.time = time;
    UpdateLeaderboard(data, height, width);
}

void DisplayLeaderboard(int height, int width) {
    clear();
    refresh();

    int size;
    Stat *leaderboard = ReadLeaderboard(height, width, size);

    string header[3] = {"No.", "Name", "Time"};
    int space[3] = {3, 10, 4};
    const int spacing = 2;

    int startX = (COLS - space[0] - space[1] - space[2] - spacing * 2) / 2;
    int startY = (LINES - NUM_LEADERBOARD * 2 - 1) / 2;

    WINDOW *win[3];
    for (int i = 0; i < 3; i++) {
        win[i] = newwin(NUM_LEADERBOARD * 2 + 1, space[i], startY, startX);
        mvwaddstr(win[i], 0, 0, header[i].c_str());

        wrefresh(win[i]);

        startX += space[i] + spacing;
    }

    for (int i = 0; i < NUM_LEADERBOARD && i < size; i++) {
        PrintInMiddle(win[0], to_string(i + 1).c_str(), i * 2 + 2);
        wrefresh(win[0]);

        mvwaddstr(win[1], i * 2 + 2, 0, leaderboard[i].name);
        wrefresh(win[1]);

        PrintInMiddle(win[2], to_string(leaderboard[i].time) + "s", i * 2 + 2);
        wrefresh(win[2]);
    }

    WINDOW *promptWin;
    PrintPrompt(promptWin, "Press any key to back to main menu", 1, LINES - 2);

    getch();

    delwin(promptWin);

    for (int i = 0; i < 3; i++) delwin(win[i]);

    clear();
    refresh();
}