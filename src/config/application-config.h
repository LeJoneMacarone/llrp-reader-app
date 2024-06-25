/*
 * Holds the necessary data for the control module's main execution routine.
 */
typedef struct {
	unsigned int readings_buffer_size;
	unsigned int crossings_buffer_size;
	char * session_data_file;
} ApplicationConfig;

/*
 * Converts a JSON string to an application configuration object.
 */
ApplicationConfig * applicationConfigFromString(const char * string);

/*
 * Extracts the application configuration from a JSON file. Returns NULL on 
 * error.
 */
ApplicationConfig * applicationConfigFromFile(const char * filePath); 

/*
 * Frees the memory allocated to the application configuration pointed by the 
 * given pointer.
 */
void applicationConfigFree(ApplicationConfig * config);
