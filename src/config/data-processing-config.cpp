#include "data-processing-config.h"
#include "../cJSON/cJSON.h"
#include "../files/files.h"

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DataProcessingConfig * dataProcessConfigFromString(const char * string) {
	cJSON * json = cJSON_Parse(string);
	cJSON * parameter;

	DataProcessingConfig * config; 
	config = (DataProcessingConfig *) malloc(sizeof(DataProcessingConfig));
	
	parameter = cJSON_GetObjectItem(json, "data-processing-logs");
	config->logs_destination = strdup(parameter->valuestring);

	parameter = cJSON_GetObjectItem(json, "crossings-export-destination");
	config->export_destination = strdup(parameter->valuestring);

	parameter = cJSON_GetObjectItem(json, "server-url");
	config->server_url = strdup(parameter->valuestring);

	parameter = cJSON_GetObjectItem(json, "station-name");
	config->station_name = strdup(parameter->valuestring);

	return config;
}

DataProcessingConfig * dataProcessConfigFromFile(const char * filePath) {
	char * buffer;

	int size = readFile(filePath, &buffer);
	
	if (size < 0) return NULL;
	
	return dataProcessConfigFromString(buffer);	
}

void dataProcessConfigFree(DataProcessingConfig * config) {
	free(config->logs_destination);
	free(config->export_destination);
	free(config->server_url);
	free(config->station_name);
	free(config);
}
