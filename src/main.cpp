#include "reader-client/reader-client.h"
#include "config/reader-client-config.h"
#include "data-processing/data-processing.h"
#include "data-processing/readings-buffer.h"

#include <pthread.h>

int main(int argc, char * argv[]) {
	ReaderClientConfig * config = fileToConfig(argv[1]);
	
	pthread_t readerClient, dataProcessing;
	readings_init();

	pthread_create(&readerClient, NULL, readerClientRun, (void *) config);
	pthread_create(&dataProcessing, NULL, dataProcessingRun, NULL);
	
	pthread_join(readerClient, NULL);
	setReaderClientDone();
	readings_print();
	
	pthread_join(dataProcessing, NULL);
}
