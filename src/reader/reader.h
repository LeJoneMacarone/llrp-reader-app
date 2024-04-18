#include <stdio.h>

#include "../../lib/ltkcpp/ltkcpp.h"

using namespace LLRP;

/*
 * Constructs and initializes a new connection with the reader, returning it's 
 * object representation. The connection can be used afterwards to send/recv 
 * messages between the client and the reader.
 */
CConnection * connectToReader(const char* hostname);

/*
 * Sends a message to the reader. Returns 0 if the message was successfully sent
 * or -1 otherwise.
 */
int sendMessage (CConnection * connection, CMessage * message);

/*
 * Waits for a message from the reader during the given timeout (in ms).
 * Returns the message or NULL in case of an error.
 */
CMessage * recvMessage(CConnection * connection, int timeoutMS);

/*
 * Equivalent to sending a message to the reader and receiving it's response.
 * Returns the message or NULL in case of an error. 
 */
CMessage * transact (CConnection * connection, CMessage * request);

/*
 * Adds a ROSpec to the reader. Equivalent to transact a ADD_ROSPEC message.
 * Returns the reponse or NULL in case of an error. 
 */
CMessage * addROSpec(CConnection * connection);
