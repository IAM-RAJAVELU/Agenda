#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
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
                printf("Current Event: %s\n", eventNode->event.name);
                printf("Start Time: %s\n", eventNode->event.startTime);
                printf("End Time: %s\n", eventNode->event.endTime);
                // Add a 3-second interval
                sleep(3);
            } else {
                printf("No event scheduled at the current time.\n");
            }
        } else {
            // Search for events based on user-specified time
            Node* eventNode = searchEventAtTime(root, input);
            if (eventNode != NULL) {
                printf("Event: %s\n", eventNode->event.name);
                printf("Start Time: %s\n", eventNode->event.startTime);
                printf("End Time: %s\n", eventNode->event.endTime);
                // Add a 3-second interval
                sleep(3);
            } else {
                printf("No event scheduled at the specified time.\n");
            }
        }
    }

    // Free memory allocated for BST
    destroyTree(root);

    return 0;
}
