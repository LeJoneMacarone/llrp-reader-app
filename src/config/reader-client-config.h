/*
 * Holds the necessary data for the reader client's main execution routine.
 */
typedef struct {
	char * reader_host;
	char * logs_destination;
	char * export_destination;
	int enable_impinj_extensions;
	int reset_to_factory_defaults;
	unsigned int inventory_duration;
	unsigned int access_report_timeout;
	unsigned int connection_attempt_timeout;
} ReaderClientConfig;

/*
 * Converts a JSON string to a reader client configuration object.
 */
ReaderClientConfig * stringToConfig(const char * string);

/*
 * Extracts the configuration from a JSON file. Returns NULL on error.
 */
ReaderClientConfig * fileToConfig(const char * filePath); 

/*
 * Frees the memory allocated to the config pointed by the given pointer.
 */
void configFree(ReaderClientConfig * config);
