// bst.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
// #define DEBUG 1
// Function to create a new node
Node* createNode(Event event) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->event = event;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

// Function to insert an event into BST
Node* insert(Node* root, Event event) {
    if (root == NULL) {
        return createNode(event);
    }

    // Compare event start times to determine insertion position
    if (strcmp(event.startTime, root->event.startTime) < 0) {
        root->left = insert(root->left, event);
    } else {
        root->right = insert(root->right, event);
    }

    return root;
}

// Function for in-order traversal of BST (for testing)
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("Event: %s, Start Time: %s, End Time: %s\n", root->event.name, root->event.startTime, root->event.endTime);
        inOrderTraversal(root->right);
    }
}

// Function to destroy the BST and free memory
void destroyTree(Node* root) {
    if (root != NULL) {
        destroyTree(root->left);
        destroyTree(root->right);
        free(root);
    }
}

Node* searchEventAtTime(Node* root, const char* currentTime) {
    // Base case: if the tree is empty or the current node is NULL
    if (root == NULL) {
        return NULL;
    }

    // Convert currentTime to minutes since midnight for easier comparison
    int currentHour, currentMinute;
    sscanf(currentTime, "%d:%d", &currentHour, &currentMinute);
    int currentTimeInMinutes = currentHour * 60 + currentMinute;

    // Convert event start time and end time to minutes since midnight for comparison
    int eventStartTimeHour, eventStartTimeMinute;
    sscanf(root->event.startTime, "%d:%d", &eventStartTimeHour, &eventStartTimeMinute);
    int eventStartTimeInMinutes = eventStartTimeHour * 60 + eventStartTimeMinute;

    int eventEndTimeHour, eventEndTimeMinute;
    sscanf(root->event.endTime, "%d:%d", &eventEndTimeHour, &eventEndTimeMinute);
    int eventEndTimeInMinutes = eventEndTimeHour * 60 + eventEndTimeMinute;

    // Check if the current time is within the range of the event's start and end times
    if (currentTimeInMinutes >= eventStartTimeInMinutes && currentTimeInMinutes <= eventEndTimeInMinutes) {
        return root; // Current time is within event's time range
    } else if (currentTimeInMinutes < eventStartTimeInMinutes) {
        return searchEventAtTime(root->left, currentTime); // Search left subtree
    } else {
        return searchEventAtTime(root->right, currentTime); // Search right subtree
    }
}


// Function to search for an event in BST based on specified time range
Node* searchEventInRange(Node* root, const char* startTime, const char* endTime) {
    // Base case: if the tree is empty or the current node is NULL
    if (root == NULL) {
        return NULL;
    }

    // Convert start time and end time to minutes since midnight for comparison
    int startHour, startMinute;
    sscanf(startTime, "%d:%d", &startHour, &startMinute);
    int startTimeInMinutes = startHour * 60 + startMinute;

    int endHour, endMinute;
    sscanf(endTime, "%d:%d", &endHour, &endMinute);
    int endTimeInMinutes = endHour * 60 + endMinute;

    // Convert event start time and end time to minutes since midnight for comparison
    int eventStartTimeHour, eventStartTimeMinute;
    sscanf(root->event.startTime, "%d:%d", &eventStartTimeHour, &eventStartTimeMinute);
    int eventStartTimeInMinutes = eventStartTimeHour * 60 + eventStartTimeMinute;

    int eventEndTimeHour, eventEndTimeMinute;
    sscanf(root->event.endTime, "%d:%d", &eventEndTimeHour, &eventEndTimeMinute);
    int eventEndTimeInMinutes = eventEndTimeHour * 60 + eventEndTimeMinute;

#ifdef DEBUG
    printf("Debug: Searching event %s in range %s - %s (Event Start Time: %d, Event End Time: %d, Start Time: %d, End Time: %d)\n", root->event.name, startTime, endTime, eventStartTimeInMinutes, eventEndTimeInMinutes, startTimeInMinutes, endTimeInMinutes);
#endif

    // Check if the event overlaps with the specified time range
    if (eventStartTimeInMinutes <= endTimeInMinutes && eventEndTimeInMinutes >= startTimeInMinutes) {
        return root; // Event overlaps with the specified time range
    } else if (eventEndTimeInMinutes < startTimeInMinutes) {
        return searchEventInRange(root->right, startTime, endTime); // Search right subtree
    } else {
        return searchEventInRange(root->left, startTime, endTime); // Search left subtree
    }
}

// Function to get the next event node in the BST
Node* getNextEventNode(Node* root, const char* currentTime) {
    // Base case: if the tree is empty or the current node is NULL
    if (root == NULL) {
        return NULL;
    }

    // Convert currentTime to minutes since midnight for easier comparison
    int currentHour, currentMinute;
    sscanf(currentTime, "%d:%d", &currentHour, &currentMinute);
    int currentTimeInMinutes = currentHour * 60 + currentMinute;

    // Traverse the BST to find the next event node
    Node* nextNode = NULL;
    while (root != NULL) {
        // Convert event start time to minutes since midnight for comparison
        int eventStartTimeHour, eventStartTimeMinute;
        sscanf(root->event.startTime, "%d:%d", &eventStartTimeHour, &eventStartTimeMinute);
        int eventStartTimeInMinutes = eventStartTimeHour * 60 + eventStartTimeMinute;

        // If the event start time is after the current time, update nextNode and move to the left subtree
        if (eventStartTimeInMinutes > currentTimeInMinutes) {
            nextNode = root;
            root = root->left;
        }
        // Otherwise, move to the right subtree
        else {
            root = root->right;
        }
    }

    return nextNode;
}
