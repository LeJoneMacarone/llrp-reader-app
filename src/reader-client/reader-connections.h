#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

/*
 * Constructs and initializes a new connection with the reader, returning it's 
 * object representation. The connection can be used afterwards to send/recv 
 * messages between the client and the reader.
 */
CConnection * connectToReader(const char* hostname);

/*
 * Checks the status of a connection to ensure it is properly established, by 
 * receiving and validating a READER_EVENT_NOTIFICATION message within a 
 * specified timeout period. Returns 0 if successfull or -1 in case of error.
 */
int checkConnectionStatus(CConnection * connection, int timeout);
