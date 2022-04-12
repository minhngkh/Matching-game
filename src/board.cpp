#include "board.hpp"

using namespace std;

bool GenerateBoard(Card **&board, int height, int width) {
    int totalCards = height * width;

    // if (totalCards > MAX_TOTAL_CARDS) {
    //     errorMsg = "Maximum size reached";
    //     return false;
    // }
    // if (totalCards < MIN_TOTAL_CARDS) {
    //     errorMsg = "Minimum size reached";
    //     return false;
    // }
    // if (totalCards % 2 == 1) {
    //     errorMsg = "product of sides must be even";
    //     return false;
    // }
    // if (height <= 0 || width <= 0) {
    //     errorMsg = "Invalid size";
    //     return false;
    // }

    // board = new Card *[height];
    // for (int i = 0; i < height; i++) {
    //     board[i] = new Card[width];
    // }

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

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            swap(board[i][j], board[heightDist(gen)][widthDist(gen)]);
        }
    }

    return true;
}

void DisplayCard(Card card) {
    wbkgd(card.win, COLOR_PAIR(0));
    box(card.win, 0, 0);
    mvwaddch(card.win, CARD_HEIGHT / 2, CARD_WIDTH / 2, card.val);

    touchwin(card.win);
    wrefresh(card.win);
    touchwin(card.win);
}

void DisplayBoard(Card **board, int boardHeight, int boardWidth) {
    // int boardHeight = sizeof(board) / sizeof(*board);
    // int boardWidth = sizeof(*board) / sizeof(**board);
    int winHeight = boardHeight * CARD_HEIGHT + (boardHeight - 1);
    int winWidth = boardWidth * CARD_WIDTH + (boardWidth - 1);
    int initY = (LINES - winHeight) / 2;
    int initX = (COLS - winWidth) / 2;

    Pos boardPos;
    boardPos.y = initY;

    for (int i = 0; i < boardHeight; i++) {
        boardPos.x = initX;

        for (int j = 0; j < boardWidth; j++) {
            board[i][j].win = newwin(CARD_HEIGHT, CARD_WIDTH, boardPos.y, boardPos.x);

            DisplayCard(board[i][j]);
            boardPos.x += CARD_WIDTH + 1;
        }

        boardPos.y += CARD_HEIGHT + 1;
    }
}

void RefreshBoard(Card **board, int boardHeight, int boardWidth) {
    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            if (board[i][j].status == "removed") continue;
            wrefresh(board[i][j].win);
        }
    }
}

bool ToggleCard(Card &card) {
    if (card.status == "selected" || card.status == "removed") 
        return false;

    if (card.status == "highlighted") {
        wbkgd(card.win, COLOR_PAIR(0));
        card.status = "none";
    } else {
        wbkgd(card.win, COLOR_PAIR(1));
        card.status = "highlighted";
    }

    wrefresh(card.win);

    return true;
}

bool UnselectCard(Card &card) {
    wbkgd(card.win, COLOR_PAIR(0));
    card.status = "none";
    wrefresh(card.win);
    return true;
}


Pos *SelectPair(Card **board, int boardHeight, int boardWidth) {
    int ch;

    // highlight the first card of the board
    Pos currPos;
    
    bool beenFound = false;
    for (int i = 0; i < boardHeight; i++) {
        if (beenFound) break;  
        for (int j = 0; j < boardWidth; j++) {
            if (board[i][j].status != "removed") {
                currPos = {i, j};
                ToggleCard(board[i][j]);
                beenFound = true;
                break;
            }
        }
    }
    int selectedCards = 0;
    Pos *selectedPos = new Pos[2];
    int initPos;

    while (selectedCards < 2) {
        ch = getch();
        switch (ch) {
            case 'a':
            case 'A':
            case KEY_LEFT:
                if (currPos.x == 0) break;

                // find the closest available card that is not yet removed
                initPos = currPos.x;
                while (currPos.x != 0) {
                    --currPos.x;
                    if (board[currPos.y][currPos.x].status != "removed") break;
                }

                if (board[currPos.y][currPos.x].status == "removed" && currPos.x == 0) {
                    // reset back to the initial pos
                    currPos.x = initPos;
                    break;
                }

                ToggleCard(board[currPos.y][initPos]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                if (currPos.x == boardWidth - 1) break;

                initPos = currPos.x;
                while (currPos.x != boardWidth - 1) {
                    ++currPos.x;
                    if (board[currPos.y][currPos.x].status != "removed") break;
                }

                if (board[currPos.y][currPos.x].status == "removed" && currPos.x == boardWidth - 1) {
                    currPos.x = initPos;
                    break;
                }
                ToggleCard(board[currPos.y][initPos]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case 'w':
            case 'W':
            case KEY_UP:
                if (currPos.y == 0) break;

                initPos = currPos.y;
                while (currPos.y != 0) {
                    --currPos.y;
                    if (board[currPos.y][currPos.x].status != "removed") break;
                }

                if (board[currPos.y][currPos.x].status == "removed" && currPos.y == 0) {
                    currPos.y = initPos;
                    break;
                }

                ToggleCard(board[initPos][currPos.x]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                if (currPos.y == boardHeight - 1) break;
            
                initPos = currPos.y;
                while (currPos.y != boardHeight - 1) {
                    ++currPos.y;
                    if (board[currPos.y][currPos.x].status != "removed") break;
                }
                
                if (board[currPos.y][currPos.x].status == "removed" && currPos.y == boardHeight - 1) {
                    currPos.y = initPos;
                    break;
                }

                ToggleCard(board[initPos][currPos.x]);
                ToggleCard(board[currPos.y][currPos.x]);

                break;

            case '\n':
                if (board[currPos.y][currPos.x].status == "selected") break;

                board[currPos.y][currPos.x].status = "selected";

                selectedPos[selectedCards] = currPos;
                ++selectedCards;

                break;

            default:
                break;
        }
    }

    return selectedPos;
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
        RemoveWin(currentCard.win);
        currentCard.status = "removed";
    }
}