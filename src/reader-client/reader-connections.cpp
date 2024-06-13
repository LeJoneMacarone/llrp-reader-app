#include "reader-connections.h"
#include "reader-messages.h"

#include <stdio.h>

#define KB 1024u
#define seconds 1000

CConnection * connectToReader(const char* hostname) {
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

