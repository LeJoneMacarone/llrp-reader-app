#include "reader-client.h"
#include "reader-connections.h"
#include "reader-config.h"
#include "reader-operations.h"
#include "reader-reports.h"
#include "../files/files.h"
#include "../config/reader-client-config.h"
#include "../data-processing/readings-buffer.h"
#include "../logs/logs.h"

#include <stdio.h>
#include <string.h>

void * readerClientRun(void * args) {
	ReaderClientConfig * config = (ReaderClientConfig *) args;
	
	FILE * out = strcmp(config->logs_destination, "stdout")
		? fopen(config->logs_destination, "w") : stdout;
	
	CConnection * connection = connectToReader(config->reader_host);
	
	if (NULL == connection) {
		log(out, ERROR, "failed to estabilish connection");
	}

    if (0 != checkConnectionStatus(connection, config->connection_attempt_timeout)) {
		log(out, ERROR, "check connection status failed");
	}

	log(out, INFO, "connection status ok");

	if (config->enable_impinj_extensions) {
		if (0 != enableImpinjExtensions(connection))
			log(out, WARN, "couldn't enable Impinj extensions");
		else log(out, INFO, "Impinj extensions enabled");
	}

	if (config->reset_to_factory_defaults) {
		if (0 != resetConfigurationToFactoryDefaults(connection))
			log(out, WARN, "couldn't reset config to factory defaults");
		else log(out, INFO, "config reseted to factory defaults");
	}

    // OPTIONAL: getCapabilities(); getReaderConfig(); setReaderConfig();

	if (0 != addROSpec(connection)) {
		log(out, ERROR, "failed adding ROSpec");
	}

	log(out, INFO, "ROSpec added successfully");

	if (0 != enableROSpec(connection)) {
		log(out, ERROR, "failed enabling ROSpec");
	}

	log(out, INFO, "ROSpec enabled successfully");

	if (0 != startROSpec(connection)) {
		log(out, ERROR, "failed starting ROSpec");
	}

	log(out, INFO, "ROSpec started successfully");

    receiveAccessReports(
			connection, 
			config->inventory_duration, 
			config->access_report_timeout,
			addReadingsFromReport
	);

	if (0 != stopROSpec(connection)) {
		log(out, ERROR, "failed stopping ROSpec");
	}

	log(out, INFO, "ROSpec stopped successfully");

	if (config->reset_to_factory_defaults) {
		if (0 != resetConfigurationToFactoryDefaults(connection))
			log(out, WARN, "couldn't reset to factory defaults");
		else log(out, INFO, "reset done successfully");
	}

    connection->closeConnectionToReader();
    delete connection;

    CXMLTextDecoder::cleanupParser();
	
	log(out, INFO, "reader client finished");

	char * readings = readings_toString();
	writeFile(config->export_destination, readings);

	free(readings);

	return NULL;
}

