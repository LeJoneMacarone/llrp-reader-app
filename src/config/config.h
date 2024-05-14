#include <iostream>

#include "../cJSON/cJSON.h"
#include "../files/files.h"

using namespace std;

typedef struct {
	const char * server_url;
	const char * reader_host;
	int enable_impinj_extensions;
	int reset_to_factory_defaults;
	unsigned int inventory_duration;
	unsigned int access_report_timeout;
	unsigned int connection_attempt_timeout;
} Configuration;

/*
 * Prints the application configuration on the terminal
 */
void printConfig(const char * filePath);

/*
 * Converts a JSON string to a configuration object.
 */
Configuration* stringToConfig(const char * string);

/*
 * Converts a JSON file to a configuration object. Returns NULL on error.
 */
Configuration * fileToConfig(const char * filePath); 
