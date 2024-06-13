#include "reader-client/reader-client.h"
#include "config/reader-client-config.h"
#include "data-processing/data-processing.h"

#include <unistd.h> 
#include <pthread.h>

int main (int argc, char * argv[]) {
	ReaderClientConfig * config = fileToConfig(argv[1]);
	
	pthread_t readerClient, dataProcessing;

	pthread_create(&readerClient, NULL, readerClientRun, (void *) config);
	pthread_create(&dataProcessing, NULL, dataProcessingRun , NULL);
	
	pthread_join(readerClient, NULL);
	setReaderClientDone();

	pthread_join(dataProcessing, NULL);
}
