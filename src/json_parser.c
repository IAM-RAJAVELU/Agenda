#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_parser.h"

#define MAX_LINE_LENGTH 100
#define MAX_EVENT_NAME_LENGTH 50
#define TIME_FORMAT_LENGTH 6

Node* parseEventsFromJSON(const char* filename) {
    // Open the JSON file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        perror("Error");
        return NULL;
    }

    // Read events from JSON file and insert them into BST
    Node* root = NULL;
    char line[MAX_LINE_LENGTH];
    int lineNum = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        lineNum++;

        // Parse the JSON line and extract event details
        char eventName[MAX_EVENT_NAME_LENGTH];
        char startTime[TIME_FORMAT_LENGTH];
        char endTime[TIME_FORMAT_LENGTH];

        int result = sscanf(line, "{\"name\": \"%49[^\"]\", \"startTime\": \"%5[^\"]\", \"endTime\": \"%5[^\"]\"}", eventName, startTime, endTime);

        if (result == 3) {
            // Create an Event struct
            Event event;
            strcpy(event.name, eventName);
            strcpy(event.startTime, startTime);
            strcpy(event.endTime, endTime);

            // Insert the event into BST
            root = insert(root, event);
        } else {
            fprintf(stderr, "Error: Invalid JSON format in file %s, line %d\n", filename, lineNum);
        }
    }

    // Close the file
    fclose(file);

    return root;
}
