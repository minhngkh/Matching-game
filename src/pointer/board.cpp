#include "board.hpp"

using namespace std;

bool GenerateBoard(Card **&board, int height, int width) {
    int totalCards = height * width;

    // Create board
    board = new Card*[width];
    for (int i = 0; i < height; i++) {
        board[i] = new Card [width];
    }

    // Initialize random device and random generator
    random_device dev;
    mt19937 gen(dev());

    // Put random chars into the board
    char beginChar = 'A';
    char endChar = 'Z';
    int charsLeft = totalCards / 2;

    for (int i = 0; i < totalCards - 1; i += 2) {
        // Randomize offsets from 'A'
        char begin = beginChar;
        char end = (endChar - beginChar) / charsLeft + beginChar;
        uniform_int_distribution<char> offsetDist(begin, end);

        beginChar = offsetDist(gen);

        board[i / width][i % width].val = beginChar;
        board[(i + 1) / width][(i + 1) % width].val = beginChar;

        ++beginChar; // no offset is included twice
        --charsLeft;
    }

    // shuffle the board
    uniform_int_distribution<int> heightDist(0, height - 1);
    uniform_int_distribution<int> widthDist(0, width - 1);

    for (int i = 0; i < height; i += 1 + NON_RANDOMNESS) {
        for (int j = 0; j < width; j += 1 + NON_RANDOMNESS) {
            swap(board[i][j], board[heightDist(gen)][widthDist(gen)]);
        }
    }

    return true;
}

void DisplayCard(Card card) {
    wbkgd(card.win.core, COLOR_PAIR(0));
    box(card.win.cover, 0, 0);
    mvwaddch(card.win.core, (CARD_HEIGHT - 2 - 1) / 2, (CARD_WIDTH - 2 - 1) / 2, card.val);

    touchwin(card.win.cover);
    touchwin(card.win.core);
    wrefresh(card.win.core);
    wrefresh(card.win.cover);
    touchwin(card.win.cover);
}

void DisplayBoard(Card **board, int boardHeight, int boardWidth) {
    // int boardHeight = sizeof(board) / sizeof(*board);
    // int boardWidth = sizeof(*board) / sizeof(**board);
    int winHeight = boardHeight * CARD_HEIGHT + (boardHeight - 1) * CARD_SPACE / 2;
    int winWidth = boardWidth * CARD_WIDTH + (boardWidth - 1) * CARD_SPACE;
    int initY = (LINES - winHeight) / 2;
    int initX = (COLS - winWidth) / 2;

    Pos boardPos;
    boardPos.y = initY;

    for (int i = 0; i < boardHeight; i++) {
        boardPos.x = initX;

        for (int j = 0; j < boardWidth; j++) {
            board[i][j].win.cover = newwin(CARD_HEIGHT, CARD_WIDTH, boardPos.y, boardPos.x);
            board[i][j].win.core = derwin(board[i][j].win.cover, CARD_HEIGHT - 2, CARD_WIDTH - 2, 1, 1);

            DisplayCard(board[i][j]);
            boardPos.x += CARD_WIDTH + CARD_SPACE;
        }
        // Because char's height is about double the width
        boardPos.y += CARD_HEIGHT + CARD_SPACE / 2;
    }
}

void RefreshBoard(Card **board, int boardHeight, int boardWidth) {
    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            if (board[i][j].status == STATUS_REMOVED) continue;
            touchwin(board[i][j].win.cover);
            wrefresh(board[i][j].win.core);
            wrefresh(board[i][j].win.cover);
        }
    }
}

bool ToggleCard(Card &card) {
    if (card.status == STATUS_SELECTED || card.status == STATUS_REMOVED) 
        return false;

    if (card.status == STATUS_HIGHLIGHTED) {
        wbkgd(card.win.core, COLOR_PAIR(0));
        card.status = STATUS_NONE;
    } else {
        wbkgd(card.win.core, COLOR_PAIR(1));
        card.status = STATUS_HIGHLIGHTED;
    }

    wrefresh(card.win.core);
    wrefresh(card.win.cover);

    return true;
}

bool UnselectCard(Card &card) {
    wbkgd(card.win.core, COLOR_PAIR(0));
    card.status = STATUS_NONE;
    wrefresh(card.win.core);
    wrefresh(card.win.cover);
    return true;
}


int GetInput(Card **board, int boardHeight, int boardWidth, Pos *selectedPos, Pos *&path, int &pathLen) {
    int ch;

    // highlight the first card of the board
    Pos currPos;
    
    bool beenFound = false;
    for (int i = 0; i < boardHeight; i++) {
        if (beenFound) break;  
        for (int j = 0; j < boardWidth; j++) {
            if (board[i][j].status != STATUS_REMOVED) {
                currPos = {i, j};
                ToggleCard(board[i][j]);
                beenFound = true;
                break;
            }
        }
    }
    int selectedCards = 0;

    while (selectedCards < 2) {
        ch = getch();
        Pos initPos = currPos;
        int limit;
        bool toToggle = false;

        switch (ch) {
            case 'a':
            case 'A':
            case KEY_LEFT:
                if (currPos.x == 0) break;

                // find the end of the offset loop
                limit = max(boardHeight - 1 - currPos.y, currPos.y) * 2 + 1;

                // start from 0 to avoid looping 0 2 times
                for (int i = 1; i < limit + 1; i++) {
                    // offset to find card in all rows
                    int offset = pow(-1, i) * (i / 2);
                    int tempPos = initPos.y + offset;

                    if (tempPos < 0 || boardHeight <= tempPos) continue;

                    currPos.y = tempPos;

                    // loop until an available card is found in row
                    while (currPos.x != 0) {
                        --currPos.x;
                        if (board[currPos.y][currPos.x].status != STATUS_REMOVED) break;
                    }

                    if (board[currPos.y][currPos.x].status == STATUS_REMOVED && currPos.x == 0) {
                        // card not found in row so reset
                        currPos.x = initPos.x;
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                } else MovingSound();

                ToggleCard(board[initPos.y][initPos.x]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                if (currPos.x == boardWidth - 1) break;

                limit = max(boardHeight - 1 - currPos.y, currPos.y) * 2 + 1;

                for (int i = 1; i < limit + 1; i++) {
                    int offset = pow(-1, i) * (i / 2);
                    int tempPos = initPos.y + offset;

                    if (tempPos < 0 || boardHeight <= tempPos) continue;

                    currPos.y = tempPos;

                    while (currPos.x != boardWidth - 1) {
                        ++currPos.x;
                        if (board[currPos.y][currPos.x].status != STATUS_REMOVED) break;
                    }

                    if (board[currPos.y][currPos.x].status == STATUS_REMOVED && currPos.x == boardWidth - 1) {
                        currPos.x = initPos.x;
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                } else MovingSound();

                ToggleCard(board[initPos.y][initPos.x]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case 'w':
            case 'W':
            case KEY_UP:
                if (currPos.y == 0) break;

                limit = max(boardWidth - 1 - currPos.x, currPos.x) * 2 + 1;

                for (int i = 1; i < limit + 1; i++) {
                    int offset = pow(-1, i) * (i / 2);
                    int tempPos = initPos.x + offset;

                    if (tempPos < 0 || boardWidth <= tempPos) continue;

                    currPos.x = tempPos;

                    while (currPos.y != 0) {
                        --currPos.y;
                        if (board[currPos.y][currPos.x].status != STATUS_REMOVED) break;
                    }

                    if (board[currPos.y][currPos.x].status == STATUS_REMOVED && currPos.y == 0) {
                        currPos.y = initPos.y;
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                } else MovingSound();

                ToggleCard(board[initPos.y][initPos.x]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                if (currPos.y == boardHeight - 1) break;

                limit = max(boardWidth - 1 - currPos.x, currPos.x) * 2 + 1;

                for (int i = 1; i < limit + 1; i++) {
                    int offset = pow(-1, i) * (i / 2);
                    int tempPos = initPos.x + offset;

                    if (tempPos < 0 || boardWidth <= tempPos) continue;

                    currPos.x = tempPos;

                    while (currPos.y != boardHeight - 1) {
                        ++currPos.y;
                        if (board[currPos.y][currPos.x].status != STATUS_REMOVED) break;
                    }

                    if (board[currPos.y][currPos.x].status == STATUS_REMOVED && currPos.y == boardHeight - 1) {
                        currPos.y = initPos.y;
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                } else MovingSound();

                ToggleCard(board[initPos.y][initPos.x]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case '\r':
            case '\n':
            case KEY_ENTER:
                if (board[currPos.y][currPos.x].status == STATUS_SELECTED) break;

                board[currPos.y][currPos.x].status = STATUS_SELECTED;

                selectedPos[selectedCards] = currPos;
                ++selectedCards;

                break;

            case 3: //^C
                return ST_FORCE_OUT;
            
            case '0':
                return ST_SURRENDER;                

            case '9':
                // clear selected state of all current cards
                UnselectCard(board[currPos.y][currPos.x]);
                for (int i = 0; i < selectedCards; i++) {
                    UnselectCard(board[selectedPos[i].y][selectedPos[i].x]);
                }
                if (FindHint(board, boardHeight, boardWidth, path, pathLen)) {
                    selectedPos[0] = path[0];
                    selectedPos[1] = path[pathLen - 1];
                    return ST_ASSISTED;
                }
                return ST_NOPAIRS;

            case '8': //endgame check
                UnselectCard(board[currPos.y][currPos.x]);
                for (int i = 0; i < selectedCards; i++) {
                    UnselectCard(board[selectedPos[i].y][selectedPos[i].x]);
                }
                if (FindHint(board, boardHeight, boardWidth, path, pathLen)) {
                    return ST_RESET;
                }
                return ST_NOPAIRS;
            default:
                break;
        }
    }

    return ST_NORMAL;
}

bool TogglePair(Card **board, Pos *pair) {
    for (int i = 0; i < 2; i++) {
        if (!UnselectCard(board[pair[i].y][pair[i].x])) return false;
    }

    return true;
}

void RemovePair(Card **board, Pos *pair) {
    for (int i = 0; i < 2; i++) {
        Card &currentCard = board[pair[i].y][pair[i].x];
        EmptyWin(currentCard.win.cover);
        currentCard.status = STATUS_REMOVED;
    }
}

// slide from right to left
void SlideBoard(Card **board, int boardWidth, Pos removedPos) {
    int y = removedPos.y;
    int x = removedPos.x + 1;

    if (x == (boardWidth) || board[y][x].status == STATUS_REMOVED) return;

    while (x < boardWidth && board[y][x].status != STATUS_REMOVED) {
        board[y][x - 1].val = board[y][x].val;
        board[y][x - 1].status = board[y][x].status;

        DisplayCard(board[y][x - 1]);

        ++x;
    }

    board[y][x - 1].status = STATUS_REMOVED;
    EmptyWin(board[y][x - 1].win.cover);
}
 
void SlideBoard(Card **board, int boardWidth, Pos *removedPos) {
    Pos posL = removedPos[0];
    Pos posR = removedPos[1];

    if (posL.x > posR.x) swap(posL, posR);

    // slide card on the right first because SlideBoard func only works with row having 1 removed card
    SlideBoard(board, boardWidth, posR);
    SlideBoard(board, boardWidth, posL);
}