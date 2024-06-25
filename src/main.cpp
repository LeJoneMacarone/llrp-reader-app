#include "reader-client/reader-client.h"
#include "config/application-config.h"
#include "config/reader-client-config.h"
#include "config/data-processing-config.h"
#include "data-processing/data-processing.h"
#include "data-processing/readings-buffer.h"
#include "data-processing/crossings-buffer.h"
#include "data-processing/event.h"

#include <stdio.h>
#include <pthread.h>

ApplicationConfig * applicationConfig;
ReaderClientConfig * readerClientConfig; 
DataProcessingConfig * dataProcessingConfig;

pthread_t readerClient, dataProcessing;

int main(int argc, char * argv[]) {
	printf("[INFO] loading config ...\n");
	applicationConfig = applicationConfigFromFile(argv[1]);
	readerClientConfig = readerClientConfigFromFile(argv[1]);
	dataProcessingConfig = dataProcessConfigFromFile(argv[1]);
	printf("[INFO] done\n");

	printf("[INFO] creating buffers ...\n");
	readings_init(applicationConfig->readings_buffer_size);
	crossings_init(applicationConfig->crossings_buffer_size);
	printf("[INFO] done\n");

	printf("[INFO] loading event data ...\n");
	importEventData(applicationConfig->session_data_file);
	printf("[INFO] done\n");

	pthread_create(
		&readerClient, NULL, 
		readerClientRun, 
		(void *) readerClientConfig
	);

	pthread_create(
		&dataProcessing, NULL, 
		dataProcessingRun, 
		(void *) dataProcessingConfig
	);

	pthread_join(readerClient, NULL);
	setReaderClientDone();
	pthread_join(dataProcessing, NULL);

	readerClientConfigFree(readerClientConfig);
	dataProcessConfigFree(dataProcessingConfig);
}
