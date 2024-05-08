#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bst.h"
#include "json_parser.h"

// Function to display current date and time
void displayCurrentDateTime() {
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    printf("Current Date and Time: %s", asctime(current_time));
}

// Function to get current time in HH:MM format
void getCurrentTime(char *currentTime) {
    time_t now = time(NULL);
    struct tm* current_time = localtime(&now);
    strftime(currentTime, 6, "%H:%M", current_time);
}

// Function to print responses based on task state
void printResponse(const char *eventName, const char *state) {
    if (strcmp(state, "done") == 0) {
        printf("Chill, you already did %s.\n", eventName);
    } else if (strcmp(state, "undone") == 0) {
        printf("Are you in the middle of %s? Or yet to do it!!(yes/no)\n", eventName);
    } else {
        printf("Invalid task state for %s: %s\n", eventName, state);
    }
}

// Function to update the state of an event based on user input
void updateEventState(Node* eventNode) {
    // Print the event details and prompt the user for input
    printResponse(eventNode->event.name, eventNode->event.state);
    // If event state is already 'done' or 'chill', no need to prompt again
    if (strcmp(eventNode->event.state, "done") == 0 || strcmp(eventNode->event.state, "Chill") == 0) {
        return;
    }
    char response[10];
    fgets(response, sizeof(response), stdin);
    if (strcmp(response, "yes\n") == 0) {
        // Update the event state to "done"
        strcpy(eventNode->event.state, "done");
        printf("Event state updated to 'done' for %s\n", eventNode->event.name);
    } else if (strcmp(response, "no\n") == 0) {
        // Update the event state to "undone"
        strcpy(eventNode->event.state, "undone");
        printf("Event state updated to 'undone' for %s\n", eventNode->event.name);
    } else {
        printf("Invalid response. Please enter 'yes' or 'no'.\n");
    }
}
// Function to calculate the time difference between two times in minutes
int timeDifferenceInMinutes(const char* startTime, const char* endTime) {
    int startHour, startMinute, endHour, endMinute;
    sscanf(startTime, "%d:%d", &startHour, &startMinute);
    sscanf(endTime, "%d:%d", &endHour, &endMinute);
    
    int startTotalMinutes = startHour * 60 + startMinute;
    int endTotalMinutes = endHour * 60 + endMinute;

    return endTotalMinutes - startTotalMinutes;
}

// Function to print reminders for events starting soon
void checkEventReminders(Node* root, const char* currentTime, int reminderMinutes) {
    Node* currentNode = root;
    while (currentNode != NULL) {
        Event currentEvent = currentNode->event;
        
        // Calculate time difference for event start
        int startDifference = timeDifferenceInMinutes(currentTime, currentEvent.startTime);
        if (startDifference > 0 && startDifference <= reminderMinutes) {
            printf("Reminder: Event '%s' is starting in %d minutes.\n", currentEvent.name, startDifference);
        }
        
        currentNode = getNextEventNode(root, currentEvent.endTime);
    }
}

int main() {
    // Display current date and time
    displayCurrentDateTime();

    // Load events from JSON file into BST
    Node* root = parseEventsFromJSON("events.json");
    if (root == NULL) {
        fprintf(stderr, "Error: Unable to load events from JSON file.\n");
        return 1;
    }

    // User input loop
    char input[20];
    while (1) {
        // Prompt user for input
        printf("Enter 'now' or a time in HH:MM format: ");
        fgets(input, sizeof(input), stdin);

        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        // Process user input
        if (strcmp(input, "now") == 0) {
            char currentTime[6];
            getCurrentTime(currentTime);
            printf("Current Time: %s\n", currentTime);

            // Search for events based on current time
            Node* eventNode = searchEventAtTime(root, currentTime);
            if (eventNode != NULL) {
                // Update event state
                updateEventState(eventNode);
            } else {
                printf("No event scheduled at the current time.\n");
            }
        } else {
            // Search for events based on user-specified time
            Node* eventNode = searchEventAtTime(root, input);
            if (eventNode != NULL) {
                // Update event state
                updateEventState(eventNode);
            } else {
                printf("No event scheduled at the specified time.\n");
            }
        }

        // Check for reminders
        char currentTime[6];
        getCurrentTime(currentTime);
        checkEventReminders(root, currentTime, 10); // Check for events starting in 10 minutes
    }

    // Free memory allocated for BST
    destroyTree(root);

    return 0;
}
