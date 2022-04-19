#include "linked-list.hpp"

bool IsEmpty(List &currList) {
    if (currList.head == NULL && currList.tail == NULL) return true;
    return false;
}

bool IsEmpty(Path &currPath) {
    if (currPath.head == NULL && currPath.tail == NULL) return true;
    return false;
}

Node *CreateNode(Card newData) {
    Node *newNode = new Node;
    newNode->data = newData;
    
    return newNode;
}

pointNode *CreateNode(Pos newData) {
    pointNode *newPNode = new pointNode;
    newPNode->data = newData;
    
    return newPNode;
}

int GetLength(List &currList) {
    int count = 0;
    Node *currNode = currList.head;

    while(currNode) {
        currNode = currNode->next;
        ++count;
    }

    return count;
}

Node *GetNode(List currList, int pos) {
    Node *currNode = currList.head;

    if (pos < 0) return NULL;
    if (pos == 0) return currList.head;

    for (int i = 1; i < pos + 1; i++) {
        if (!currNode->next) return NULL;
        currNode = currNode->next;
    }

    return currNode;
}

pointNode *GetNode(Path currPath, int pos) {
    pointNode *currPNode = currPath.head;

    if (pos < 0) return NULL;
    if (pos == 0) return currPath.head;

    for (int i = 1; i < pos + 1; i++) {
        if (!currPNode->next) return NULL;
        currPNode = currPNode->next;
    }

    return currPNode;
}

Node *GetNode(List *board, Pos currPos) {
    return GetNode(board[currPos.y], currPos.x);
}

void Push(List &currList, Card newData) {
    Node *newNode = CreateNode(newData);

    newNode->next = currList.head;
    currList.head = newNode;

    if (IsEmpty(currList)) currList.tail = newNode;
}

void InsertAfter(List &currList, Node *currNode, Card newData) {
    if (currNode == NULL) return;
 
    Node* newNode = CreateNode(newData);

    newNode->next = currNode->next;
    currNode->next = newNode;

    if (currList.tail == currNode) currList.tail = newNode;
}

// add node to the end of the list
void Append(List &currList , Card newData) {
    Node *newNode = CreateNode(newData);

    if (IsEmpty(currList)) {
        currList.tail = currList.head = newNode;
        return ;
    }

    currList.tail->next = newNode;
    currList.tail = newNode;
}

void Append(Path &currPath, Pos newData) {
    pointNode *newPNode = CreateNode(newData);

    if (IsEmpty(currPath)) {
        currPath.tail = currPath.head = newPNode;
        return ;
    }

    currPath.tail->next = newPNode;
    currPath.tail = newPNode;
}

// remove the last node
bool Pop(List &currList) {
    if (IsEmpty(currList)) return false;

    if (currList.head == currList.tail) {
        delete currList.head;
        currList.head = currList.tail;

        return true;    
    }

    Node *currNode = currList.head;

    while (currNode->next != currList.head) {
        currNode = currNode->next;
    }

    currNode->next = NULL;
    delete currList.tail;
    currList.tail = currNode;

    return true;
}

bool Pop(List &currList, int pos) {
    if (IsEmpty(currList)) return false;

    if (pos < 0) return false;
    if (pos == 0) {
        Node *tempNode = currList.head->next;
        delete currList.head;
        currList.head = tempNode;

        return true;
    }

    // pos > 0
    Node *prevNode = GetNode(currList, pos - 1);
    
    if (!prevNode) return false;
    if (!prevNode->next) return false;

    Node *currNode = prevNode->next;

    if (currNode == currList.tail) return Pop(currList);
    
    prevNode->next = currNode->next;
    delete currNode;

    return true;
}

bool Pop(List *&board, Pos currPos) {
    return Pop(board[currPos.y], currPos.x); 
}