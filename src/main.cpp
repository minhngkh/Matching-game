#include <ncurses.h>
#include <iostream>
#include "board.hpp"

using namespace std;

int startX = 0;
int startY = 0;

#define SIZE 6

struct Pos {
    int x,y;
};

struct WinCard {
    bool selected = false;
    bool highlight = false;
    WINDOW *win;
};

void GenerateCard(WinCard card, char content) {
    wbkgd(card.win, COLOR_PAIR(0));
    box(card.win, 0, 0);
    mvwaddch(card.win, 1, 2, content);

    wrefresh(card.win);
}

void TryToggleCard(WinCard *card) {
    if (card->highlight && !(card->selected)) {
        wbkgd(card->win, COLOR_PAIR(0));
        card->highlight = false;
    } else {
        wbkgd(card->win, COLOR_PAIR(1));
        card->highlight = true;
    }

    wrefresh(card->win);
}

int main() {
    initscr();
    cbreak();
    noecho();
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_CYAN, COLOR_WHITE);
    refresh();

    char **board = GenerateBoard(SIZE);
    WinCard cards[SIZE][SIZE];

    startY = (LINES - (SIZE * 5 + (SIZE - 1))) / 2;
    for (int i = 0; i < SIZE; i++) {

        startX = (COLS - (SIZE * 5 + (SIZE - 1))) / 2;
        for (int j = 0; j < SIZE; j++) {
            cards[i][j].win = newwin(3, 5, startY, startX);

            GenerateCard(cards[i][j], board[i][j]);

            startX += 6;
        }

        startY += 4;
    }

    getch();

    int ch;
    Pos curPos = {0, 0};
    TryToggleCard(&cards[0][0]);
    int selectedCards = 0;

    while (selectedCards < 2) {
        ch = getch();
        switch (ch) {
            case KEY_LEFT:
                if (curPos.x == 0) break;

                TryToggleCard(&cards[curPos.y][curPos.x]);
                --curPos.x;
                TryToggleCard(&cards[curPos.y][curPos.x]);

                break;
            case KEY_RIGHT:
                if (curPos.x == SIZE - 1) break;

                TryToggleCard(&cards[curPos.y][curPos.x]);
                ++curPos.x;
                TryToggleCard(&cards[curPos.y][curPos.x]);

                break;
            case KEY_UP:
                if (curPos.y == 0) break;

                TryToggleCard(&cards[curPos.y][curPos.x]);
                --curPos.y;
                TryToggleCard(&cards[curPos.y][curPos.x]);

                break;
            case KEY_DOWN:
                if (curPos.y == SIZE - 1) break;

                TryToggleCard(&cards[curPos.y][curPos.x]);
                ++curPos.y;
                TryToggleCard(&cards[curPos.y][curPos.x]);

                break;
            case '\n':
                cards[curPos.y][curPos.x].selected = true;
                ++selectedCards;

                break;
            default: 
                break;
        }
    }

    getch();
    endwin();

    return 0;
}