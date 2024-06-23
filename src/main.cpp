#include "reader-client/reader-client.h"
#include "config/reader-client-config.h"
#include "config/data-processing-config.h"
#include "data-processing/data-processing.h"
#include "data-processing/readings-buffer.h"
#include "data-processing/event.h"

#include <stdio.h>
#include <pthread.h>

ReaderClientConfig * readerClientConfig; 
DataProcessingConfig * dataProcessingConfig;

pthread_t readerClient, dataProcessing;

int option;

int main(int argc, char * argv[]) {
	printf("[INFO] Loading config ...\n");
	readerClientConfig = readerClientConfigFromFile(argv[1]);
	dataProcessingConfig = dataProcessConfigFromFile(argv[1]);
	printf("[INFO] Done\n");

	readings_init();

	printf("[INFO] Loading event data ...\n");
	importEventData("DATA_SESSION.json");
	printf("[INFO] Done\n");

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
}
