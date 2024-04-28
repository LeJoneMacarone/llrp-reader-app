#include <stdio.h>

#include "../../lib/ltkcpp/ltkcpp.h"
#include "../../lib/ltkcpp/impinj_ltkcpp.h"

using namespace LLRP;

/*
 * Equivalent to sending a message to the reader and receiving it's response.
 * Returns the message or NULL in case of an error. 
 */
CMessage * transact (CConnection * connection, CMessage * request);

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
 * TODO: add documentation
 */
int checkLLRPStatus (CLLRPStatus * pLLRPStatus, const char * pWhatStr);

/* 
 * Converts a CEPCData to a string. Equivalent to converting an array of bits 
 * to an array of characters.
 *
 * TODO: provide references for the conversion algorithm  
 */
void formatOneEPC(CParameter *pEPCParameter, char *buf, int buflen);

/*
 * TODO: add documentation
 */
void printOneTagReportData(CTagReportData * pTagReportData);

/*
 * TODO: add documentation
 */
void printTagReportData(CRO_ACCESS_REPORT * accessReport);

/*
 * TODO: add documentation
 */
void printXMLMessage(CMessage * message);
