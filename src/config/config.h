#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "../cJSON/cJSON.h"

using namespace std;

/*
 * Reads a file given buy filePath and copies its' contents to buffer.
 * Returns the size of the buffer or -1 if an error occured.
 */
int readFile(char* filePath, char** buffer);

/*
 * Prints the application configuration on the terminal
 */
void printConfig(const char* filePath);
