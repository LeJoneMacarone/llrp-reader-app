#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Reads a file given by filePath and copies its' contents to buffer.
 * Returns the size of the buffer or -1 if an error occured.
 */
int readFile(const char* filePath, char** buffer);

/*
 * Writes the content to the file given by filePath.
 * The file is created if it doesn't exist.
 */
void writeFile(const char* filePath, const char* content);

/*
 * Writes the content at the end of the file given by filePath.
 * The file is created if it doesn't exist.
 */
void appendFile(const char* filePath, const char* content); 

