#pragma once

#include "global.hpp"
#include "screen.hpp"
#include <random>
#include <string>

bool GenerateBoard(Card **&board, int height, int width);

void DisplayCard(Card card);

void DisplayBoard(Card **board, int boardHeight, int boardWidth);

void RefreshBoard(Card **board, int boardHeight, int boardWidth);

bool ToggleCard(Card &card);

bool TogglePair(Card **board, Pos *pair);

std::string GetInput(Card **board, int boardHeight, int boardWidth, Pos *selectedPos);

void RemovePair(Card **board, Pos* pair);

void SlideBoard(int **board, int boardHeight, int boardWidth, Pos removedPos);

void SlideBoard(int **board, int boardHeight, int boardWidth, Pos *removedPos);