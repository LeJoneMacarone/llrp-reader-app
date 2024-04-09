#include <iostream>

#include "../cJSON/cJSON.h"
#include "../files/files.h"

using namespace std;

typedef struct {
	char* serverURL;
} Configuration;

/*
 * Prints the application configuration on the terminal
 */
void printConfig(const char* filePath);

/*
 * Converts a JSON string to a configuration object.
 */
Configuration* stringToConfig(const char* string);

/*
 * Converts a JSON file to a configuration object. Returns NULL on error.
 */
Configuration* fileToConfig(const char* filePath); 
