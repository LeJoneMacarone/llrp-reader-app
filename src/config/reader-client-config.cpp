#include "reader-client-config.h"
#include "../cJSON/cJSON.h"
#include "../files/files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ReaderClientConfig * stringToConfig(const char * string) {
	cJSON * json = cJSON_Parse(string);

	ReaderClientConfig * config = new ReaderClientConfig();
	
	config->reader_host 
		= cJSON_GetObjectItem(json, "reader-host")->valuestring;
	config->inventory_duration 
		= cJSON_GetObjectItem(json, "inventory-duration")->valueint;
	config->access_report_timeout 
		= cJSON_GetObjectItem(json, "access-report-timeout")->valueint;
	config->connection_attempt_timeout 
		= cJSON_GetObjectItem(json, "connection-attempt-timeout")->valueint;

	cJSON * json_bool;
	
	json_bool = cJSON_GetObjectItem(json, "enable-impinj-extensions");
	config->enable_impinj_extensions = cJSON_IsTrue(json_bool);
	json_bool = cJSON_GetObjectItem(json, "reset-to-factory-defaults");
	config->reset_to_factory_defaults = cJSON_IsTrue(json_bool);

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
