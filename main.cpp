#include<iostream>
#include"elem/board.hpp"

using namespace std;

int main() {
    char **board;
    board = GenerateBoard(6);
    DisplayBoard(board, 6);
    
    return 0;
}