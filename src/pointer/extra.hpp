#include "string"
#include<fstream>
#include<iostream>
using namespace std;

struct Stat{
string name;
int time;
};

void Swap(Stat &x, Stat &y);
void SortAscendingOrder(Stat *a, int n);
void UpdateLeaderboard(Stat player);
Stat *ReadLeaderboard();