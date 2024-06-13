#include "../config/reader-client-config.h"

#include "../../lib/ltkcpp/ltkcpp.h"

using namespace LLRP;

/*
 * TODO: add documentation
 */
void * readerClientRun(void * config);

/*
 * Constructs and initializes a new connection with the reader, returning it's 
 * object representation. The connection can be used afterwards to send/recv 
 * messages between the client and the reader.
 */
CConnection * connectToReader(const char* hostname);

/*
 * TODO: add documentation
 */
int checkConnectionStatus(CConnection * connection, int timeout);

/*
 * TODO: add documentation
 */
int enableImpinjExtensions(CConnection * connection);

/*
 * Adds a ROSpec to the reader. Equivalent to transact a ADD_ROSPEC message.
 * Returns the reponse or NULL in case of an error. 
 */
int addROSpec(CConnection * connection);

/*
 * TODO: add documentation
 */
int enableROSpec(CConnection * connection);

/*
 * TODO: add documentation
 */
int stopROSpec(CConnection * connection);

/*
 * TODO: add documentation
 */
void receiveAccessReports(CConnection * connection, int duration, int timeout);
