#pragma once

#include "global.hpp"
#include "screen.hpp"
#include <ncurses.h>
#include <random>

#define MAX_TOTAL_CARDS 100
#define MIN_TOTAL_CARDS 2

bool GenerateBoard(Card **&board, int height, int width);

#define CARD_WIDTH 5
#define CARD_HEIGHT 3
void DisplayCard(Card card);

void DisplayBoard(Card **board, int boardHeight, int boardWidth);

void RefreshBoard(Card **board, int boardHeight, int boardWidth);

bool ToggleCard(Card &card);

bool TogglePair(Card **board, Pos *pair);

Pos *SelectPair(Card **board, int boardHeight, int boardWidth);

void RemovePair(Card **board, Pos* pair);
