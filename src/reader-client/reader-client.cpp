#include "reader-client.h"
#include "reader-connections.h"
#include "reader-config.h"
#include "reader-operations.h"
#include "reader-reports.h"
#include "../config/reader-client-config.h"
#include "../data-processing/readings.h"
#include <stdio.h>

void * readerClientRun(void * args) {
	ReaderClientConfig * config = (ReaderClientConfig *) args;
	CConnection * connection = connectToReader(config->reader_host);
	
	if (NULL == connection) {
		printf("[ERROR] failed to estabilish connection\n");
	}

    if (0 != checkConnectionStatus(connection, config->connection_attempt_timeout)) {
		printf("[ERROR] check connection status failed\n");
	}

	printf("[INFO] connection status ok\n");

	if (config->enable_impinj_extensions) {
		if (0 != enableImpinjExtensions(connection))
			printf("[WARN] couldn't enable Impinj extensions\n");
		else printf("[INFO] Impinj extensions enabled\n");
	}

	if (config->reset_to_factory_defaults) {
		if (0 != resetConfigurationToFactoryDefaults(connection))
			printf("[WARN] couldn't reset config to factory defaults\n");
		else printf("[INFO] config reseted to factory defaults\n");
	}

    // OPTIONAL: getCapabilities(); getReaderConfig(); setReaderConfig();

	if (0 != addROSpec(connection)) {
		printf("[ERROR] failed adding ROSpec\n");
	}

	printf("[INFO] ROSpec added successfully\n");

	if (0 != enableROSpec(connection)) {
		printf("[ERROR] failed enabling ROSpec\n");
	}

	printf("[INFO] ROSpec enabled successfully\n");

	if (0 != startROSpec(connection)) {
		printf("[ERROR] failed starting ROSpec\n");
	}

	printf("[INFO] ROSpec started successfully\n");

    receiveAccessReports(
			connection, 
			config->inventory_duration, 
			config->access_report_timeout,
			saveAccessReport
	);

	if (0 != stopROSpec(connection)) {
		printf("[ERROR] failed stopping ROSpec\n");
	}

	printf("[INFO] ROSpec stopped successfully\n");

	if (config->reset_to_factory_defaults) {
		if (0 != resetConfigurationToFactoryDefaults(connection))
			printf("[WARN] couldn't reset to factory defaults\n");
		else printf("[INFO] reset done successfully\n");
	}

    connection->closeConnectionToReader();
    
    delete connection;

    CXMLTextDecoder::cleanupParser();
	
	printf("[INFO] reader client finished\n");

	return NULL;
}

