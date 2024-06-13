#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

/* 
 * Converts a CEPCData parameter to a string, effectivaly converting an array 
 * of bits to an array of characters.
 */
void formatOneEPC(CParameter *pEPCParameter, char *buf, int buflen);

/*
 * Extracts, processes and stores (in the readings buffer) the access report 
 * data obtained from a RO_ACCESS_REPORT object.
 */
void saveAccessReport(CRO_ACCESS_REPORT * report);

/*
 * Responsible for receving access reports from the reader while the given 
 * duration wasn't reached. The way the reports are processed and handled is 
 * up to the accessReportHandler. The timeout defines the interval between 
 * consecutive reports.
 */
void receiveAccessReports(
	CConnection * connection, 
	int duration, 
	int timeout,
	void (* accessReportHandler) (CRO_ACCESS_REPORT *)
);
