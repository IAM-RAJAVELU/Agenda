// bst.h

#ifndef BST_H
#define BST_H


// Event structure
typedef struct {
    char name[50];
    char startTime[6];
    char endTime[6];
    char state[10]; // Add state member to track the task state
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
Node* getNextEventNode(Node* root, const char* currentTime);


#endif
