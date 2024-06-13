#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

/*
 * TODO: add documentation
 */
void saveAccessReport(CRO_ACCESS_REPORT * report);

/* 
 * Converts a CEPCData parameter to a string. Equivalent to converting an 
 * array of bits to an array of characters.
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

/*
 * Responsible for receving access reports from the reader while the given 
 * duration wasn't reached. The way the reports are processed and handled is 
 * up to the accessReportHandler. The timeout defines the time between one 
 * report and the next one.
 */
void receiveAccessReports(
	CConnection * connection, 
	int duration, 
	int timeout,
	void (* accessReportHandler) (CRO_ACCESS_REPORT *)
);
