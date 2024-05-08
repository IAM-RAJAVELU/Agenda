#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "json_parser.h"

Node* parseEventsFromJSON(const char* filename) {
    // Open the JSON file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    // Read the JSON file line by line
    char line[512]; // Assuming each line in JSON file is at most 512 characters
    char day[20];    // Buffer to store the day of the week
    Node* root = NULL;

    // Get today's day of the week
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    strftime(day, sizeof(day), "%A", current_time);
    printf("Today is %s\n", day);

    // Look for today's events in the JSON data
    int isDayFound = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if the line contains today's day of the week
        if (strstr(line, day) != NULL) {
            isDayFound = 1;
            printf("Found events for %s\n", day);
            break;
        }
    }

    if (!isDayFound) {
        fprintf(stderr, "Error: Today's events not found in JSON file.\n");
        fclose(file);
        return NULL;
    }

    // Parse today's events from JSON
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if the line contains the start of an event array
        if (strstr(line, "[") != NULL) {
            break;
        }
    }

    // Parse events and insert them into BST
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if the line contains the end of the event array
        if (strstr(line, "]") != NULL) {
            break;
        }

        // Parse event details
        char eventName[50];
        char startTime[6];
        char endTime[6];
        char state[10];
        if (sscanf(line, " {\"name\": \"%49[^\"]\", \"startTime\": \"%5[^\"]\", \"endTime\": \"%5[^\"]\", \"state\": \"%7[^\"]\"},", eventName, startTime, endTime, state) == 4) {
            // Create an Event struct
            Event event;
            strcpy(event.name, eventName);
            strcpy(event.startTime, startTime);
            strcpy(event.endTime, endTime);
            strcpy(event.state, state);

            // Insert the event into BST
            root = insert(root, event);
            printf("Inserted event: %s, %s - %s, State: %s\n", event.name, event.startTime, event.endTime, event.state);
        } else {
            fprintf(stderr, "Error: Invalid event format in JSON file.\n");
        }
    }

    // Close the file
    fclose(file);

    return root;
}
