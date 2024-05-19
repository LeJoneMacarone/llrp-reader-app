#include "reader.h"
#include "reader-utils.h"
#include "reader-messages.h"

#include "../../lib/ltkcpp/impinj_ltkcpp.h"

#include <stdio.h>

#define KB 1024u
#define seconds 1000

static int messageID = 0;

int getMessageID() {
    return messageID++;
}

CConnection* connectToReader(const char* hostname) {
	// the registry is used for decoding messages
 	CTypeRegistry* typeRegistry = getTheTypeRegistry();
 	enrollCoreTypesIntoRegistry(typeRegistry);
 
 	// initialize a connection object with the type registry and 32KB of max
	// frame size for send/recv
 	CConnection* connection = new CConnection(typeRegistry, 32u*KB);
 
 	if (NULL == connection) return NULL;

 	// open the connection with the provided hostname
 	int status = connection->openConnectionToReader(hostname);

	if (0 != status) {
 		delete connection;
 		return NULL;
 	}

 	return connection;
 }

int checkConnectionStatus(CConnection * connection, int timeout) {
	// expect the message within 10 seconds
    CMessage * message = recvMessage(connection, timeout * seconds);
	
    // make sure there's a message before proceding
    if (NULL == message) return -1;

    // make sure the message it's a READER_EVENT_NOTIFICATION
    if (&CREADER_EVENT_NOTIFICATION::s_typeDescriptor != message -> m_pType) {
        delete message;
        return -1;
    }

    CREADER_EVENT_NOTIFICATION * notification 
        = (CREADER_EVENT_NOTIFICATION *) message;

    CReaderEventNotificationData * data 
        = notification->getReaderEventNotificationData();

    // make sure there's data before proceding    
    if (NULL == data) {
        delete message;
        return -1;
    }

    // the ConnectionAttemptEvent parameter must be present.
    CConnectionAttemptEvent * event = data->getConnectionAttemptEvent();
    if (NULL == event) {
        delete message;
        return -1;
    }

    // the status in the ConnectionAttemptEvent parameter
    // must indicate connection success
    if (ConnectionAttemptStatusType_Success != event -> getStatus()) {
        delete message;
        return -1;
    }
    
    delete message;

    return 0;
}

int enableImpinjExtensions(CConnection * connection) {
    // compose the command message
    CIMPINJ_ENABLE_EXTENSIONS * command = new CIMPINJ_ENABLE_EXTENSIONS();
    command->setMessageID(getMessageID());
    
    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received something
    if (NULL == responseMessage) return -1;

    CIMPINJ_ENABLE_EXTENSIONS_RESPONSE * response 
        = (CIMPINJ_ENABLE_EXTENSIONS_RESPONSE *) responseMessage;

    // Check the LLRPStatus parameter.
    if (0 != checkLLRPStatus(
        response->getLLRPStatus(), 
        "enableImpinjExtensions"
    )) {
        delete responseMessage;
        return -1;
    }

    delete responseMessage;

    return 0;
}

int resetConfigurationToFactoryDefaults(CConnection * connection) {
    // compose the command message
    CSET_READER_CONFIG * command;
    command = new CSET_READER_CONFIG();
    command -> setMessageID(getMessageID());
    command -> setResetToFactoryDefault(1);

    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received a message
    if (NULL == responseMessage) return -1;

    CSET_READER_CONFIG_RESPONSE * response 
        = (CSET_READER_CONFIG_RESPONSE * ) responseMessage;
    
    // check the LLRPStatus parameter
    if (0 != checkLLRPStatus(
        response->getLLRPStatus(),
        "resetConfigurationToFactoryDefaults"
    )) {
        /* checkLLRPStatus already tattled */
        delete responseMessage;
        return -1;
    }

    delete responseMessage;
    
    return 0;
}

// TODO: add possibility to add the spec from a XML file
int addROSpec(CConnection * connection) {
	// initialize fields and parameters of the ROSpec, from the most internal 
	// to the the most external
	
	// ROSpecStartTrigger
	CROSpecStartTrigger * startTrigger = new CROSpecStartTrigger();
	startTrigger->setROSpecStartTriggerType(ROSpecStartTriggerType_Null);
	
	// ROSpecStopTrigger
	CROSpecStopTrigger * stopTrigger = new CROSpecStopTrigger();
	stopTrigger->setROSpecStopTriggerType(ROSpecStopTriggerType_Null);
	stopTrigger->setDurationTriggerValue(0);

	// ROBoundarySpec = (ROSpecStartTrigger, ROSpecStopTrigger)
	CROBoundarySpec * boundarySpec = new CROBoundarySpec();
	boundarySpec->setROSpecStartTrigger(startTrigger);
	boundarySpec->setROSpecStopTrigger(stopTrigger);
	
	// AISpecStopTrigger
	CAISpecStopTrigger * aiSpecStopTrigger = new CAISpecStopTrigger();
	aiSpecStopTrigger->setAISpecStopTriggerType(AISpecStopTriggerType_Null);
	aiSpecStopTrigger->setDurationTrigger(0);

	// InventoryParameterSpec 
	CInventoryParameterSpec * inventoryParameterSpec 
		= new CInventoryParameterSpec();
	inventoryParameterSpec->setInventoryParameterSpecID(1234);
	inventoryParameterSpec->setProtocolID(AirProtocols_EPCGlobalClass1Gen2);
	
	// AntennaIDs
    llrp_u16v_t antennaIDs = llrp_u16v_t(2);
    antennaIDs.m_pValue[0] = 1;
    antennaIDs.m_pValue[1] = 2;

	// AISpec = (AISpecStopTrigger, InventoryParameterSpec, AntennaIDs)
	CAISpec * aiSpec = new CAISpec();
	aiSpec->setAntennaIDs(antennaIDs);
	aiSpec->setAISpecStopTrigger(aiSpecStopTrigger);
	aiSpec->addInventoryParameterSpec(inventoryParameterSpec);

	// ROSpec = (ROSpecID, Priority, CurrentState, ROBoundarySpec, AISpec)
	CROSpec * roSpec = new CROSpec();
	roSpec->setROSpecID(1111);
	roSpec->setPriority(0);
	roSpec->setROBoundarySpec(boundarySpec);
	roSpec->setCurrentState(ROSpecState_Disabled);
	roSpec->addSpecParameter(aiSpec);

	// append the ROSpec ta a message
	CADD_ROSPEC * message = new CADD_ROSPEC();
	message->setROSpec(roSpec);
	message->setMessageID(0);

	// send the message
	CADD_ROSPEC_RESPONSE * response 
		= (CADD_ROSPEC_RESPONSE *) transact(connection, message);
	
	delete message;
	
    if(NULL == response) return -1;

    if(0 != checkLLRPStatus(response->getLLRPStatus(), "addROSpec"))
        return -1;

    delete response;

    return 0;
}

int enableROSpec(CConnection * connection) {
    // compose the command message
    CENABLE_ROSPEC * command = new CENABLE_ROSPEC();
    command -> setMessageID(getMessageID());
    command -> setROSpecID(1111);

    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received a message
    if (NULL == responseMessage) return -1;

    CENABLE_ROSPEC_RESPONSE * response 
        = (CENABLE_ROSPEC_RESPONSE *) responseMessage;

    // check the LLRPStatus parameter.
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "enableROSpec"
    )) {
        delete responseMessage;
        return -1;
    }

    delete responseMessage;

    return 0;
}

int startROSpec(CConnection * connection) {
    // compose the command message
    CSTART_ROSPEC * command = new CSTART_ROSPEC();
    command -> setMessageID(getMessageID());
    command -> setROSpecID(1111);

    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received the message
    if (NULL == responseMessage) return -1;

    CSTART_ROSPEC_RESPONSE * response
        = (CSTART_ROSPEC_RESPONSE * ) responseMessage;

    // check the LLRPStatus parameter. 
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "startROSpec"
    )) {
        delete responseMessage;
        return -1;
    }

    delete responseMessage;
    
    return 0;
}

int stopROSpec(CConnection * connection) {
    // compose the command message
    CSTOP_ROSPEC * command = new CSTOP_ROSPEC();
    command -> setMessageID(getMessageID());
    command -> setROSpecID(1111);

   	// send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received the message
    if (NULL == responseMessage) return -1;

    CSTOP_ROSPEC_RESPONSE * response
        = (CSTOP_ROSPEC_RESPONSE * ) responseMessage;
    
    // check the LLRPStatus parameter. 
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "stopROSpec"
    )) {
        delete responseMessage;
        return -1;
    }
    delete responseMessage;

    return 0;
}

void receiveAccessReports(
		CConnection * connection, 
		int duration, 
		int timeout,
		void (* accessReportHandler) (CRO_ACCESS_REPORT *)
	) {
    int done = 0;
    time_t startTime = time(NULL);
    time_t tempTime;
    
    const CTypeDescriptor * messageType;

    // keep receiving messages until done or until something bad happens
    while (!done) {
        CMessage * message = recvMessage(connection, timeout);

		// validate the timestamp
        tempTime = time(NULL);
        if (difftime(tempTime, startTime) > duration) done = 1;
        
		// make sure we received a message
        if (NULL == message) continue;

        // handle messages by checking their types
        messageType = message->m_pType;
        if (&CRO_ACCESS_REPORT::s_typeDescriptor == messageType) {
            CRO_ACCESS_REPORT * report = (CRO_ACCESS_REPORT * ) message;
			// TODO: write report to a shared variable
            
			accessReportHandler(report);
        
		// TODO: handle various types of messages (e.g. antenna events)
        } else {
            printf("[WARN] Unexpected message\n");
        }

        delete message;
    }
}

int readerClientRun(ReaderClientConfig * config) {
	CConnection * connection = connectToReader(config->reader_host);
	
	if (NULL == connection) {
		printf("[ERROR] failed to estabilish connection\n");
		return 0;
	}

    if (0 != checkConnectionStatus(connection, config->connection_attempt_timeout)) {
		printf("[ERROR] check connection status failed\n");
		return 1;
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
		return 2;
	}

	printf("[INFO] ROSpec added successfully\n");

	if (0 != enableROSpec(connection)) {
		printf("[ERROR] failed enabling ROSpec\n");
		return 3;
	}

	printf("[INFO] ROSpec enabled successfully\n");

	if (0 != startROSpec(connection)) {
		printf("[ERROR] failed starting ROSpec\n");
		return 4;
	}

	printf("[INFO] ROSpec started successfully\n");

    receiveAccessReports(
			connection, 
			config->inventory_duration, 
			config->access_report_timeout,
			printTagReportData
	);

	if (0 != stopROSpec(connection)) {
		printf("[ERROR] failed stopping ROSpec\n");
		return 2;
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
	
	printf("[INFO] finish\n");

    return 0;
}

