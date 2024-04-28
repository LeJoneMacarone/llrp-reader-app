#include "reader.h"

#define KB 1024u
#define seconds 1000

static int messageID = 0;

int getMessageID() {
    return messageID++;
}

CConnection* connectToReader(const char* hostname) {
 	// TODO: change printfs to logs

	// the registry is used for decoding messages
 	CTypeRegistry* typeRegistry = getTheTypeRegistry();
 	enrollCoreTypesIntoRegistry(typeRegistry);
 
 	// initialize a connection object with the type registry and 32KB of max
	// frame size for send/recv
 	CConnection* connection = new CConnection(typeRegistry, 32u*KB);
 
 	if (NULL == connection) {
 		printf("ERROR: new CConnection failed\n");
 		return NULL;
 	}

 	// open the connection with the provided hostname
 	int status = connection->openConnectionToReader(hostname);

	if (0 != status) {
 		printf(
			"ERROR: connection failed: %s (%i)\n", 
			connection->getConnectError(), 
 			status
 		);
 		delete connection;
 		return NULL;
 	}
 
 	return connection;
 }

int checkConnectionStatus(CConnection * connection) {
    // expect the message within 10 seconds
    CMessage * message = recvMessage(connection, 10 * seconds);
	
    // make sure there's a message before proceding
    if (NULL == message) {
        // TODO: log that checkConnectionStatus failed
        printf("ERROR: checkConnectionStatus failed\n");
        delete message;
        return -1;
    }

    // make sure the message it's a READER_EVENT_NOTIFICATION
    if (&CREADER_EVENT_NOTIFICATION::s_typeDescriptor != message -> m_pType) {
        // TODO: log that checkConnectionStatus failed
        printf("ERROR: checkConnectionStatus failed\n");
        delete message;
        return -1;
    }

    CREADER_EVENT_NOTIFICATION * notification 
        = (CREADER_EVENT_NOTIFICATION *) message;

    CReaderEventNotificationData * data 
        = notification->getReaderEventNotificationData();

    // make sure there's data before proceding    
    if (NULL == data) {
        // TODO: log that checkConnectionStatus failed
        printf("ERROR: checkConnectionStatus failed\n");
        delete message;
        return -1;
    }

    // the ConnectionAttemptEvent parameter must be present.
    CConnectionAttemptEvent * event = data->getConnectionAttemptEvent();
    if (NULL == event) {
        // TODO: log that checkConnectionStatus failed
        printf("ERROR: checkConnectionStatus failed\n");
        delete message;
        return -1;
    }

    // the status in the ConnectionAttemptEvent parameter
    // must indicate connection success
    if (ConnectionAttemptStatusType_Success != event -> getStatus()) {
        // TODO: log that checkConnectionStatus failed
        printf("ERROR: checkConnectionStatus failed\n");
        delete message;
        return -1;
    }
    
    delete message;

    // TODO: log connection status ok
    printf("INFO: connection ok\n");

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
    if (NULL == responseMessage) {
        /* transact already tattled */
        return -1;
    }

    CIMPINJ_ENABLE_EXTENSIONS_RESPONSE * response 
        = (CIMPINJ_ENABLE_EXTENSIONS_RESPONSE *) responseMessage;

    // Check the LLRPStatus parameter.
    if (0 != checkLLRPStatus(
        response->getLLRPStatus(), 
        "enableImpinjExtensions"
    )) {
        /* checkLLRPStatus already tattled */
        delete responseMessage;
        return -1;
    }

    delete responseMessage;

    /*
     * Tattle progress, maybe
     */

    // TODO: log that extensions were enabled
    printf("INFO: Impinj extensions enabled\n");

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
    if (NULL == responseMessage) {
        /* transact already tattled */
        return -1;
    }

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
    
    // TODO: log success
    printf("INFO: Connection success!\n");
    
    return 0;
}

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
	
	// TODO: log errors (i.e. NULL response)
    if(NULL == response)
        return -1;

    if(0 != checkLLRPStatus(response->getLLRPStatus(), "addROSpec"))
        return -1;

    delete response;

	// TODO: log the ROSpec was added
    printf("INFO: ROSpec added\n");

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
    if (NULL == responseMessage) {
        /* transact already tattled */
        return -1;
    }

    CENABLE_ROSPEC_RESPONSE * response 
        = (CENABLE_ROSPEC_RESPONSE *) responseMessage;

    // check the LLRPStatus parameter.
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "enableROSpec"
    )) {
        /* checkLLRPStatus already tattled */
        delete responseMessage;
        return -1;
    }

    delete responseMessage;

    // TODO: log ROSpec enabled
    printf("INFO: ROSpec enabled\n");

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
    if (NULL == responseMessage) {
        /* transact already tattled */
        return -1;
    }

    CSTART_ROSPEC_RESPONSE * response
        = (CSTART_ROSPEC_RESPONSE * ) responseMessage;

    
    // check the LLRPStatus parameter. 
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "startROSpec"
    )) {
        /* checkLLRPStatus already tattled */
        delete responseMessage;
        return -1;
    }
    delete responseMessage;
    
    // TODO: log started ROSpec
    printf("INFO: ROSpec started\n");

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
    if (NULL == responseMessage) {
        /* transact already tattled */
        return -1;
    }

    CSTOP_ROSPEC_RESPONSE * response
        = (CSTOP_ROSPEC_RESPONSE * ) responseMessage;
    
    // check the LLRPStatus parameter. 
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "stopROSpec"
    )) {
        /* checkLLRPStatus already tattled */
        delete responseMessage;
        return -1;
    }
    delete responseMessage;
    
    // TODO: log stopped ROSpec
    printf("INFO: ROSpec stopped\n");

    return 0;
}

void awaitAndPrintReport(CConnection * connection, int timeout) {
    int done = 0;
    time_t startTime = time(NULL);
    time_t tempTime;
    
    const CTypeDescriptor * messageType;

    // keep receiving messages until done or until something bad happens
    while (!done) {
        /*
         * Wait up to 1 second for a report.  Check
         * That way, we can check the timestamp even if
         * there are no reports coming in
         */
        CMessage * message = recvMessage(connection, 1 * seconds);

        // validate the timestamp
        tempTime = time(NULL);
        if (difftime(tempTime, startTime) > timeout) {
            done = 1;
        }

        // make sure we received a message
        if (NULL == message) {
            continue;
        }

        // handle messages by checking their types
        messageType = message->m_pType;
        if (&CRO_ACCESS_REPORT::s_typeDescriptor == messageType) {
            
            CRO_ACCESS_REPORT * report = (CRO_ACCESS_REPORT * ) message;
            printTagReportData(report);

        // TODO: handle various types of messages (e.g. antenna events)
        } else {
            // TODO: log that an unexpected message appeared
            printf("Unexpected message\n");
        }

        delete message;
    }
}


int readerClientRun(const char * pReaderHostName) {
    CConnection * connection = connectToReader(pReaderHostName);

    if (0 != checkConnectionStatus(connection)) return 1;

    //if (0 != enableImpinjExtensions(connection)) return 2;

    if (0 != resetConfigurationToFactoryDefaults(connection)) return 3;

    // OPTIONAL: getCapabilities(); getReaderConfig(); setReaderConfig();

    if (0 != addROSpec(connection)) return 4;

    if (0 != enableROSpec(connection)) return 5;

    if (0 != startROSpec(connection)) return 6;

    awaitAndPrintReport(connection, 60);

    if (0 != stopROSpec(connection)) return 7;

    // TODO: log cleaning up reader
    printf("INFO: reseting reader to factory defauls...\n");

    resetConfigurationToFactoryDefaults(connection);

    // TODO: log finish
    printf("INFO: finish\n");

    connection->closeConnectionToReader();
    
    delete connection;

    // delete typeRegistry;

    CXMLTextDecoder::cleanupParser();

    return 0;
}

