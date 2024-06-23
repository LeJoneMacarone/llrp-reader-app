/*
 * Holds the necessary data for the data processing module's main execution 
 * routine.
 */
typedef struct {
	char * logs_destination;
	char * export_destination;
} DataProcessingConfig;

/*
 * Converts a JSON string to a data processing configuration object.
 */
DataProcessingConfig * dataProcessConfigFromString(const char * string);

/*
 * Extracts the data processing configuration from a JSON file. Returns NULL 
 * on error.
 */
DataProcessingConfig * dataProcessConfigFromFile(const char * filePath); 

/*
 * Frees the memory allocated to the data processing configuration pointed by 
 * the given pointer.
 */
void dataProcessConfigFree(DataProcessingConfig * config);
