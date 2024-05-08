// bst.h

#ifndef BST_H
#define BST_H

// Event structure
typedef struct {
    char name[50];
    char startTime[6]; // Assuming HH:MM format
    char endTime[6];   // Assuming HH:MM format
} Event;

// Node structure
typedef struct Node {
    Event event;
    struct Node* left;
    struct Node* right;
} Node;

// Function declarations
Node* createNode(Event event);
Node* insert(Node* root, Event event);
Node* searchEventAtTime(Node* root, const char* currentTime);
Node* searchEventInRange(Node* root, const char* startTime, const char* endTime);
void inOrderTraversal(Node* root);
void destroyTree(Node* root);

#endif
