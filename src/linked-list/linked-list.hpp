#include "global.hpp"

struct Node {
    Card data;
    Node *next = NULL;
};

struct List {
    Node *head = NULL;
    Node *tail = NULL;
};

Node *CreateNode(Card newData);

int GetLength(List &currList);

Node *GetNode(List currList, int pos);

Node *GetNode(List *board, Pos currPos);

void Push(List &currList, Card newData);

void InsertAfter(List &currList, Node *currNode, Card newData);

void Append(List &currList , Card newData);

bool Pop(List &currList);
bool Pop(List &currList, int pos);
bool Pop(List *&board, Pos currPos);