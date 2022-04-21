#pragma once

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
pointNode *GetNode(Path currPath, int pos);

// return pointer to a node at pos of the list
Node *GetNode(List currList, int pos);
// return pointer to a node at currPos of the array of lists
Node *GetNode(List *board, Pos currPos);

// Push newNode to the start of the list
void Push(List &currList, Card newData);

// insert a node after another node
void InsertAfter(List &currList, Node *currNode, Card newData);

// Insert a node to the end of the list
void Append(List &currList , Card newData);
void Append(Path &currPath, Pos newData);

// Remove the last node of the list
bool Pop(List &currList);
// remove the node at pos of the list
bool Pop(List &currList, int pos);
// remvoe the node at currPos of the array of lists
bool Pop(List *&board, Pos currPos);