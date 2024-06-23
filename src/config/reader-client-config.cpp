#include "reader-client-config.h"
#include "../cJSON/cJSON.h"
#include "../files/files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ReaderClientConfig * stringToConfig(const char * string) {
	cJSON * json = cJSON_Parse(string);
	cJSON * parameter;

	ReaderClientConfig * config = new ReaderClientConfig();
	
	parameter = cJSON_GetObjectItem(json, "reader-host");
	config->reader_host = parameter->valuestring;

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

ReaderClientConfig * fileToConfig(const char * filePath) {
	char * buffer;

	int size = readFile(filePath, &buffer);
	
	if (size < 0) return NULL;
	
	return stringToConfig(buffer);	
}

void configFree(ReaderClientConfig * config) {
	free(config->reader_host);
	free(config);
}
