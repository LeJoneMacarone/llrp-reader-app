/*
 * Holds the necessary data for the reader client's execution routine.
 */
typedef struct {
	const char * reader_host;
	int enable_impinj_extensions;
	int reset_to_factory_defaults;
	unsigned int inventory_duration;
	unsigned int access_report_timeout;
	unsigned int connection_attempt_timeout;
} ReaderClientConfig;

/*
 * Converts a JSON string to a configuration object.
 */
ReaderClientConfig * stringToConfig(const char * string);

/*
 * Converts a JSON file to a configuration object. Returns NULL on error.
 */
ReaderClientConfig * fileToConfig(const char * filePath); 
