#include "global.hpp"

struct Node {
    Card data;
    Node *next = NULL;
};

struct List {
    Node *head = NULL;
    Node *tail = NULL;
};

struct pointNode {
    Pos data;
    pointNode *next = NULL;
};

struct Path {
    pointNode *head = NULL;
    pointNode *tail = NULL;
};

Node *CreateNode(Card newData);
pointNode *CreatePNode(Pos newData);

int GetLength(List &currList);

Node *GetNode(List currList, int pos);

Node *GetNode(List *board, Pos currPos);

void Push(List &currList, Card newData);

void InsertAfter(List &currList, Node *currNode, Card newData);

void Append(List &currList , Card newData);
void Append(Path &currPath, Pos newData);

bool Pop(List &currList);
bool Pop(List &currList, int pos);
bool Pop(List *&board, Pos currPos);