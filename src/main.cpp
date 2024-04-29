#include "config/config.h"
#include "internet/internet.h"
#include "logs/logs.h"
#include "reader/reader.h"

int main (int argc, char *argv[]) {
	FILE * logFile = fopen("logs/log.txt", "+a");

	// configure log messages	
	recorder_t * consoleRecorder = recorder_init(stdout, NULL),
			   * fileRecorder = recorder_init(logFile, add_timestamp);
	
	logger_t * logger = logger_instance();
	logger_add(logger, *consoleRecorder);
	logger_add(logger, *fileRecorder);

	logger_log(logger, INFO, "logging status ok, application starting");

	// run reader client
	readerClientRun(argv[1]);
}
