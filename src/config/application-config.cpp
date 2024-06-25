#include "application-config.h"
#include "../cJSON/cJSON.h"
#include "../files/files.h"

#include <string.h>

ApplicationConfig * applicationConfigFromString(const char * string) {
	cJSON * json = cJSON_Parse(string);
	cJSON * parameter;

	ApplicationConfig * config;
	config = (ApplicationConfig *) malloc(sizeof(ApplicationConfig));

	parameter = cJSON_GetObjectItem(json, "readings-buffer-size");
	config->readings_buffer_size = parameter->valueint;

	parameter = cJSON_GetObjectItem(json, "readings-buffer-size");
	config->crossings_buffer_size = parameter->valueint;

	parameter = cJSON_GetObjectItem(json, "session-data-file");
	config->session_data_file = strdup(parameter->valuestring);

	return config;
}

ApplicationConfig * applicationConfigFromFile(const char * filePath) {
	char * buffer;

	int size = readFile(filePath, &buffer);
	
	if (size < 0) return NULL;
	
	return applicationConfigFromString(buffer);	
}

void applicationConfigFree(ApplicationConfig * config) {
	free(config->session_data_file);
	free(config);
}
