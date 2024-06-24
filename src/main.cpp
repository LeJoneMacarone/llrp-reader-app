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

int main(int argc, char * argv[]) {
	printf("[INFO] loading config ...\n");
	readerClientConfig = readerClientConfigFromFile(argv[1]);
	dataProcessingConfig = dataProcessConfigFromFile(argv[1]);
	printf("[INFO] done\n");

	readings_init(256);

	printf("[INFO] loading event data ...\n");
	importEventData("DATA_SESSION.json");
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
}
