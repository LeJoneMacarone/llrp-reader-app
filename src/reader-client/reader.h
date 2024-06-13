#include "../config/reader-client-config.h"

#include "../../lib/ltkcpp/ltkcpp.h"

using namespace LLRP;

/**
 * Runs the reader client with the specified configuration, establishing a 
 * connection to a reader, configures it based on the provided settings, 
 * manages the lifecycle of a ROSpec (Reader Operation Specification) and 
 * starts an inventory. All the provided readings are stored in a buffer.
 */
void * readerClientRun(void * args);

/*
 * Constructs and initializes a new connection with the reader, returning it's 
 * object representation. The connection can be used afterwards to send/recv 
 * messages between the client and the reader.
 */
CConnection * connectToReader(const char* hostname);

/*
 * Checks the status of a connection to ensure it is properly established, by 
 * receiving and validating a READER_EVENT_NOTIFICATION message within a 
 * specified timeout period.
 */
int checkConnectionStatus(CConnection * connection, int timeout);

/**
 * Enables Impinj extensions on the specified connection, by sending a 
 * command to the reader and verifies the response to ensure the extensions 
 * are successfully enabled.
 */
int enableImpinjExtensions(CConnection * connection);

/*
 * Adds a ROSpec to the reader. Equivalent to transacting an ADD_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int addROSpec(CConnection * connection);

/*
 * Enables a ROSpec on the reader. Equivalent to transact an ENABLE_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int enableROSpec(CConnection * connection);

/*
 * Stops a ROSpec on the reader. Equivalent to transact an STOP_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int stopROSpec(CConnection * connection);

/*
 * Starts a ROSpec on the reader. Equivalent to transact an START_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int startROSpec(CConnection * connection);

/*
 * Responsible for receving access reports from the reader while the given 
 * duration wasn't reached. The way the reports are processed and handled is 
 * up to the accessReportHandler. The timeout defines the time between one 
 * report and the next one.
 */
void receiveAccessReports(CConnection * connection, int duration, int timeout);
