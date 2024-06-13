#include "internet/internet.h"
#include "logs/logs.h"
#include "reader-client/reader.h"
#include "data-processing/readings.h"

#include <unistd.h> 
#include <pthread.h>

int main (int argc, char *argv[]) {
	ReaderClientConfig * config = fileToConfig(argv[1]);
	
	pthread_t readerClient, dataProcessing;
	pthread_create(&readerClient, NULL, readerClientRun, (void *) config);
	pthread_create(&dataProcessing, NULL, dataProcessingRun , NULL);
	
	pthread_join(readerClient, NULL);
	setReaderClientDone();

	pthread_join(dataProcessing, NULL);
	readings_print();
}
