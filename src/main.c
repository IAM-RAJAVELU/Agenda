#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep function
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
        printf("Are you in the middle of %s?  Or yet to do it!!(yes/no)\n", eventName);
    } else {
        printf("Invalid task state for %s: %s\n", eventName, state);
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
                // Print response and ask for user's confirmation
                printResponse(eventNode->event.name, eventNode->event.state);
                
                // Prompt for user's response
                char response[10];
                fgets(response, sizeof(response), stdin);
                if (strcmp(response, "yes\n") == 0) {
                    // Handle 'yes' response
                    // For example, update the state of the event to "done"
                } else if (strcmp(response, "no\n") == 0) {
                    // Handle 'no' response
                    // For example, keep the state of the event as "undone"
                } else {
                    printf("Invalid response. Please enter 'yes' or 'no'.\n");
                }
                sleep(3); // Wait for 3 seconds
            } else {
                printf("No event scheduled at the current time.\n");
            }
        } else {
            // Search for events based on user-specified time
            Node* eventNode = searchEventAtTime(root, input);
            if (eventNode != NULL) {
                printResponse(eventNode->event.name, eventNode->event.state);
                sleep(3); // Wait for 3 seconds
            } else {
                printf("No event scheduled at the specified time.\n");
            }
        }
    }

    // Free memory allocated for BST
    destroyTree(root);

    return 0;
}
