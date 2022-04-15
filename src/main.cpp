#include "global.hpp"
#include "board.hpp"
#include "screen.hpp"
#include "path.hpp"

using namespace std;

int main() {

    // Setup terminal
    initscr();

    raw();                 // using raw buffer
    noecho();              // to not print buffer on screen
    start_color();         // enable color support
    keypad(stdscr, TRUE);  // enable support for arrow keys
    curs_set(0);           // disable typing indicator
    init_pair(1, COLOR_CYAN, COLOR_WHITE);

    refresh();

    int height, width;

    // input board's size
    WINDOW *inWin;
    PrintPrompt(inWin, "Size:", 2);

    echo();
    cbreak();
    wrefresh(inWin);

    mvwscanw(inWin, 1, (COLS - 3) / 2, "%i %i", &height, &width);

    noecho();
    raw();
    
    RemoveWin(inWin);

    // Generate board
    Card **board;
    GenerateBoard(board, height, width);

    // Play
    bool gameOver = false;

    while (!gameOver) {
        WINDOW *background;
        DisplayBackground(background);
        DisplayBoard(board, height, width);

        // Prompt before start
        PrintPrompt(inWin, "Press any key to continue", 1, LINES - 1);

        getch();

        RemoveWin(inWin);

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

            string gameState = GetInput(board, height, width, selectedPos);
            if (gameState == "force out") return 0;
            if (gameState == "surrender") {
                gameOver = true;
                break;
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
        break;
    }

    clear();
    
    PrintPrompt(inWin, "GAME OVER");

    getch();
    endwin();

    return 0;
}