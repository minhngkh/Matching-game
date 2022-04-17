#include "global.hpp"

struct Node {
    Card data;
    Node *next = NULL;
};

struct List {
    Node *head = NULL;
    Node *tail = NULL;
};

