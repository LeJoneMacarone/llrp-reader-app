#include "reader-client-config.h"
#include "../cJSON/cJSON.h"
#include "../files/files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ReaderClientConfig * readerClientConfigFromString(const char * string) {
	cJSON * json = cJSON_Parse(string);
	cJSON * parameter;

	ReaderClientConfig * config; 
	config = (ReaderClientConfig *) malloc(sizeof(ReaderClientConfig));
	
	parameter = cJSON_GetObjectItem(json, "reader-host");
	config->reader_host = strdup(parameter->valuestring);

	parameter = cJSON_GetObjectItem(json, "reader-client-logs");
	config->logs_destination = strdup(parameter->valuestring);

	parameter = cJSON_GetObjectItem(json, "readings-export-destination");
	config->export_destination = strdup(parameter->valuestring);

	parameter = cJSON_GetObjectItem(json, "inventory-duration");
	config->inventory_duration = parameter->valueint;
	
	parameter = cJSON_GetObjectItem(json, "access-report-timeout");
	config->access_report_timeout = parameter->valueint;
	
	parameter = cJSON_GetObjectItem(json, "connection-attempt-timeout");
	config->connection_attempt_timeout = parameter->valueint;

	parameter = cJSON_GetObjectItem(json, "enable-impinj-extensions");
	config->enable_impinj_extensions = cJSON_IsTrue(parameter);
	
	parameter = cJSON_GetObjectItem(json, "reset-to-factory-defaults");
	config->reset_to_factory_defaults = cJSON_IsTrue(parameter);

	return config;
}

ReaderClientConfig * readerClientConfigFromFile(const char * filePath) {
	char * buffer;

	int size = readFile(filePath, &buffer);
	
	if (size < 0) return NULL;
	
	return readerClientConfigFromString(buffer);	
}

void readerClientConfigFree(ReaderClientConfig * config) {
	free(config->reader_host);
	free(config->logs_destination);
	free(config->export_destination);
	free(config);
}
