#include "config.h"

void printConfig(const char * filePath) {
	char * buffer;
	int size = readFile(filePath, &buffer);
		
	if (size < 0) return;

	cJSON * json = cJSON_Parse(buffer);

	cJSON * iterator = json->child;

	while (iterator) {
		cout << iterator->string << " = " << iterator->valuestring << "\n";
		iterator = iterator->next;
	}

	cJSON_free(json);
}

Configuration * stringToConfig(const char * string) {
	cJSON * json = cJSON_Parse(string);
                                            
	Configuration * config = new Configuration();
	
	config->serverURL = cJSON_GetObjectItem(json, "server-url")->valuestring;

	return config;
}

Configuration * fileToConfig(const char * filePath) {
	char * buffer;

	int size = readFile(filePath, &buffer);
	
	if (size < 0) return NULL;
	
	return stringToConfig(buffer);	
}
