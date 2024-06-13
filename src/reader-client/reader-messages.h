#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

/*
 * TODO: add documentation
 */
int getMessageID();

/*
 * Equivalent to sending a message to the reader and receiving it's response.
 * Returns the message or NULL in case of an error. 
 */
CMessage * transact (CConnection * connection, CMessage * request);

/*
 * Sends a message to the reader. Returns 0 if the message was successfully 
 * sent or -1 otherwise.
 */
int sendMessage (CConnection * connection, CMessage * message);

/*
 * Waits for a message from the reader during the given timeout (in ms).
 * Returns the message or NULL in case of an error.
 */
CMessage * recvMessage(CConnection * connection, int timeoutMS);

/*
 * TODO: add documentation
 */
int checkLLRPStatus (CLLRPStatus * pLLRPStatus, const char * pWhatStr);

/*
 * TODO: add documentation
 */
void printXMLMessage(CMessage * message);
