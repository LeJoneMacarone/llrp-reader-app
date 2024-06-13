#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

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
 * Checks the LLRP status parameter in a response to ensure the operation was 
 * successful. If the status indicates failure, it prints an error message 
 * and description. Returns 0 if successfull, -1 if the status parameter is 
 * missing or -2 if the status indicates failure.
 */
int checkLLRPStatus (CLLRPStatus * pLLRPStatus, const char * pWhatStr);

/*
 * Called for getting a new ID to construct a new LLRP message. 
 */
int getMessageID();

/*
 * Prints the XML representation of a message.
 */
void printXMLMessage(CMessage * message);
