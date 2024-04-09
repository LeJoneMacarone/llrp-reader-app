#include "files.h"

int readFile(const char* filePath, char** buffer){
    FILE* file = fopen(filePath , "r");
    
    if (file == NULL) return -1;

    fseek(file, 0, SEEK_END); 

    size_t fileSize = ftell(file); 
		
    fseek(file, 0, SEEK_SET);

    *buffer = (char*) malloc(fileSize);
    int size = fread(*buffer, 1, fileSize, file);

    fclose(file);

    return size;
}

void writeFile(const char* filePath, const char* content) {
	FILE* file = fopen(filePath, "w");
	
	fwrite(content, sizeof(char), strlen(content), file);

	fclose(file);
}

void appendFile(const char* filePath, const char* content) {
	FILE* file = fopen(filePath, "a");

	fwrite(content, sizeof(char), strlen(content), file);

	fclose(file);
}
