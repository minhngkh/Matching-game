#include"board.hpp"
#include<random>

void MulPrint(string content, int times) {
    for (int i = 0; i < times; i++) {
        cout << content;
    }
}

char **GenerateBoard(int size) {
    char **board = new char*[size];
    for (int i = 0; i < size; i++) {
        board[i] = new char[size];
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        board[i][j] = 'A' + (i * size + j) / 2;
        }
    }

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(0, size-1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        swap(board[i][j], board[dist(rng)][dist(rng)]);
        }
    }

    

    return board;
}

void DisplayBoard(char **board, int size) {
    MulPrint(" ---", size);
    
    for (int i = 0; i < size; i++) {
        
        cout << endl;

        for (int j = 0; j < size; j++) {
            if (j == 0) {
                cout << "| ";
            }

            cout << board[i][j] << " | ";
        }
        cout << endl;

        MulPrint(" ---", size);
    } 
}