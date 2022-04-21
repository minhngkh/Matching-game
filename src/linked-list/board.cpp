#include "board.hpp"

using namespace std;

bool GenerateBoard(List *&board, int height, int width) {
    int totalCards = height * width;
    // Create board
    board = new List[height];

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

        Card newCard;
        newCard.val = beginChar;
        Append(board[i / width], newCard);
        Append(board[(i + 1) / width], newCard);

        ++beginChar; // no offset is included twice
        --charsLeft;
    }

    // shuffle the board
    uniform_int_distribution<int> heightDist(0, height - 1);
    uniform_int_distribution<int> widthDist(0, width - 1);

    for (int i = 0; i < height; i += 1 + NON_RANDOMNESS) {
        for (int j = 0; j < width; j += 1 + NON_RANDOMNESS) {
            Node *tempNode1 = GetNode(board, {i, j});
            Node *tempNode2 = GetNode(board, {heightDist(gen), widthDist(gen)});

            swap(tempNode1->data.val, tempNode2->data.val);
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

void DisplayBoard(List *board, int boardHeight, int boardWidth) {
    int winHeight = boardHeight * CARD_HEIGHT + (boardHeight - 1) * CARD_SPACE / 2;
    int winWidth = boardWidth * CARD_WIDTH + (boardWidth - 1) * CARD_SPACE;
    int initY = (LINES - winHeight) / 2;
    int initX = (COLS - winWidth) / 2;

    Pos boardPos;
    boardPos.y = initY;

    for (int i = 0; i < boardHeight; i++) {
        boardPos.x = initX;

        Node *currNode = GetNode(board, {i, 0});

        while (currNode) {
            Card &currCard = currNode->data;

            currCard.win.cover = newwin(CARD_HEIGHT, CARD_WIDTH, boardPos.y, boardPos.x);
            currCard.win.core = derwin(currCard.win.cover, CARD_HEIGHT - 2, CARD_WIDTH - 2, 1, 1);

            DisplayCard(currCard);

            currNode = currNode->next;

            boardPos.x += CARD_WIDTH + CARD_SPACE;
        }
        // Because char's height is about double the width so i divide the spacing by 2
        boardPos.y += CARD_HEIGHT + CARD_SPACE / 2;
    }
}

void RefreshBoard(List *board, int boardHeight) {
    for (int i = 0; i < boardHeight; i++) {
        Node *currNode = GetNode(board, {i, 0});

        while (currNode) {
            Card &currCard = currNode->data;
            currNode = currNode->next;

            if (currCard.status == STATUS_REMOVED) continue;
            touchwin(currCard.win.cover);
            wrefresh(currCard.win.core);
            wrefresh(currCard.win.cover);

        }
    }
}

bool ToggleCard(Card &card) {
    // only allow highilighted and none status
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


int GetInput(List *board, int boardHeight, int boardWidth, Pos *selectedPos, Path &currPath) {
    int ch;

    // highlight the first card of the board
    Pos currPos;
    
    bool beenFound = false;
    for (int i = 0; i < boardHeight; i++) {
        Node *currNode = GetNode(board, {i, 0});

        if (beenFound) break;  
        for (int j = 0; j < boardWidth; j++) {
            Card &currCard = currNode->data;

            if (currCard.status != STATUS_REMOVED) {
                currPos = {i, j};
                ToggleCard(currCard);
                beenFound = true;
                break;
            }

            currNode = currNode->next;
        }
    }

    int selectedCards = 0;

    // stop function after getting a pair
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
                        if (GetNode(board, currPos)->data.status != STATUS_REMOVED) break;
                    }

                    if (GetNode(board, currPos)->data.status == STATUS_REMOVED && currPos.x == 0) {
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

                ToggleCard(GetNode(board, initPos)->data);
                ToggleCard(GetNode(board, currPos)->data);

                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                if (currPos.x == boardWidth - 1) break;

                limit = max(boardHeight - 1 - currPos.y, currPos.y) * 2 + 1;
                
                Node *currNode;
                for (int i = 1; i < limit + 1; i++) {
                    int offset = pow(-1, i) * (i / 2);
                    int tempPos = initPos.y + offset;

                    if (tempPos < 0 || boardHeight <= tempPos) continue;

                    currPos.y = tempPos;

                    // Only situation that this can be optimized because we access card in the list direction
                    Node *currNode = GetNode(board, currPos);

                    while (currPos.x != boardWidth - 1) {
                        ++currPos.x;
                        currNode = currNode->next;
                        if (currNode->data.status != STATUS_REMOVED) break;
                    }

                    if (GetNode(board, currPos)->data.status == STATUS_REMOVED && currPos.x == boardWidth - 1) {
                        currPos.x = initPos.x;
                        currNode = GetNode(board, currPos);
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                    currNode = GetNode(board, currPos);
                } else MovingSound();

                ToggleCard(GetNode(board, initPos)->data);
                ToggleCard(GetNode(board, currPos)->data);

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
                        if (GetNode(board, currPos)->data.status != STATUS_REMOVED) break;
                    }

                    if (GetNode(board, currPos)->data.status == STATUS_REMOVED && currPos.y == 0) {
                        currPos.y = initPos.y;
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                } else MovingSound();

                ToggleCard(GetNode(board, initPos)->data);
                ToggleCard(GetNode(board, currPos)->data);

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
                        if (GetNode(board, currPos)->data.status != STATUS_REMOVED) break;
                    }

                    if (GetNode(board, currPos)->data.status == STATUS_REMOVED && currPos.y == boardHeight - 1) {
                        currPos.y = initPos.y;
                    } else {
                        toToggle = true;
                        break;
                    }
                }

                if (!toToggle) {
                    currPos = initPos;
                } else MovingSound();

                ToggleCard(GetNode(board, initPos)->data);
                ToggleCard(GetNode(board, currPos)->data);

                break;

            case '\r':
            case '\n':
            case KEY_ENTER:
                if (GetNode(board, currPos)->data.status == STATUS_SELECTED) break;

                GetNode(board, currPos)->data.status = STATUS_SELECTED;

                selectedPos[selectedCards] = currPos;
                ++selectedCards;

                break;

            case 3: //^C
                return ST_FORCE_OUT;
            
            case '0': // Surrender
                return ST_SURRENDER;                

            case '9': // Hint
                // clear selected state of all current cards
                UnselectCard(GetNode(board, currPos)->data);
                for (int i = 0; i < selectedCards; i++) {
                    UnselectCard(GetNode(board, selectedPos[i])->data);
                }
                
                // check if there is any valid pair
                if (FindHint(board, boardHeight, boardWidth, currPath)) {
                    selectedPos[0] = currPath.head->data;
                    selectedPos[1] = currPath.tail->data;
                    return ST_ASSISTED;
                }
                return ST_NOPAIRS;

            case '8': //endgame check
                // pretty much the same as the above
                UnselectCard(GetNode(board, currPos)->data);
                for (int i = 0; i < selectedCards; i++) {
                    UnselectCard(GetNode(board, selectedPos[i])->data);
                }

                if (FindHint(board, boardHeight, boardWidth, currPath)) {
                    return ST_RESET;
                }
                return ST_NOPAIRS;

            default:
                break;
        }
    }

    return ST_NORMAL;
}

bool TogglePair(List *board, Pos *pair) {
    for (int i = 0; i < 2; i++) {
        if (!UnselectCard(GetNode(board, pair[i])->data)) return false;
    }

    return true;
}

void RemovePair(List *board, Pos *pair) {
    for (int i = 0; i < 2; i++) {
        Card &currentCard = GetNode(board, pair[i])->data;
        EmptyWin(currentCard.win.cover);
        currentCard.status = STATUS_REMOVED;
    }
}

// slide from right to left
void SlideBoard(List *board, Pos removedPos) {
    Node *currNode = GetNode(board, removedPos);


    // passing val and status of a card from  right to left
    while (currNode->next && currNode->next->data.status != STATUS_REMOVED) {
        currNode->data.val = currNode->next->data.val;
        currNode->data.status = currNode->next->data.status;
        
        DisplayCard(currNode->data);

        currNode = currNode->next;
    }

    // remove the last one after sliding
    currNode->data.status = STATUS_REMOVED;
    EmptyWin(currNode->data.win.cover);
}
 
void SlideBoard(List *board, Pos *removedPos) {
    Pos posL = removedPos[0];
    Pos posR = removedPos[1];

    if (posL.x > posR.x) swap(posL, posR);

    // slide card on the right first because SlideBoard func only works with row having 1 removed card
    SlideBoard(board, posR);
    SlideBoard(board, posL);
}