#include "test.hpp"

using namespace std;

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

void GenerateTest(Card **&board, int &height, int &width) {
    height = 5;
    width = 8;

    board = new Card*[width];
    for (int i = 0; i < height; i++) {
        board[i] = new Card [width];
    }

    ifstream ifs("src/test.txt");

    string line;
    int i = 0;
    while (getline(ifs, line)) {
        for (int j = 0; j < width; j++) {
            if (line[j] == '-') board[i][j].status = STATUS_REMOVED;
            else board[i][j].val = line[j];
        }
        ++i;
    }

    ifs.close();
}