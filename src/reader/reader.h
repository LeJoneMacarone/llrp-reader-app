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
 * Sends a message to the reader. Returns 0 if the message was sent successfully
 * and -1 otherwise.
 */
int sendMessage (CConnection * connection, CMessage * message);

/*
 * Returns a message sent by the reader during the given timeout
 */
CMessage * recvMessage(CConnection * connection, int timeoutMS);
