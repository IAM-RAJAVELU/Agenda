#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "bst.h"

// Function prototype for parsing events from a JSON file and loading them into BST
Node* parseEventsFromJSON(const char* filename);

#endif /* JSON_PARSER_H */
