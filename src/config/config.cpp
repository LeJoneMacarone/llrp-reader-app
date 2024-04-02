#include "config.h"

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

void printConfig(const char* filePath){
	char* buffer;
	int size = readFile(filePath, &buffer);
		
	if (size < 0) return;

	cJSON* json = cJSON_Parse(buffer);

	// const char* parameter = "server-url";

	cJSON* iterator = json->child;

	while (iterator) {
		cout << iterator->string << " = " << iterator->valuestring << "\n";
		iterator = iterator->next;
	}

	cJSON_free(json);
}
