#include "reader-reports.h"
#include "reader-messages.h"
#include "../data-processing/readings-buffer.h"

#include <bits/types/struct_timeval.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <list>
using namespace std;

uint64_t getCurrenTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

	uint64_t microseconds 
		= (uint64_t) tv.tv_sec * 1000000 + (uint64_t) tv.tv_usec;
    return microseconds;
}

void formatOneEPC(CParameter *pEPCParameter, char *buf, int buflen) {
    char * p = buf;
    int bufsize = buflen;
    int written = 0;

    if (NULL != pEPCParameter) {
        llrp_u96_t my_u96;
        llrp_u1v_t my_u1v;
        llrp_u8_t * epcValue = NULL;
        
		unsigned int n, i;
		
        const CTypeDescriptor * type;
        type = pEPCParameter->m_pType;

        if (&CEPC_96::s_typeDescriptor == type) {
            CEPC_96 * pEPC_96;

            pEPC_96 = (CEPC_96 *) pEPCParameter;
            my_u96 = pEPC_96->getEPC();
            epcValue = my_u96.m_aValue;
            n = 12u;
        } else if (&CEPCData::s_typeDescriptor == type) {
        	CEPCData * pEPCData;

            pEPCData = (CEPCData *) pEPCParameter;
            my_u1v = pEPCData->getEPC();
            epcValue = my_u1v.m_pValue;
            n = (my_u1v.m_nBit + 7u) / 8u;
		}

        if (NULL != epcValue) {
            for (i = 0; i < n; i++) {
                if (bufsize > 2) {
                    written = snprintf(p, bufsize, "%02X", epcValue[i]);
                    bufsize -= written;
                    p += written;
                }
            }
        } else {
            written = snprintf(p, bufsize, "%s", "---unknown-epc-data-type---");
            bufsize -= written;
            p += written;
        }
    } else {
        written = snprintf(p, bufsize, "%s", "--null epc---");
        bufsize -= written;
        p += written;
    }

    buf[buflen-1] = '\0';
}

void addReadingsFromReport(CRO_ACCESS_REPORT * report) {
	list<CTagReportData *>::iterator data;

    printf(
		"[INFO] number of entries: %i\n", 
		report->countTagReportData()
	);

	for (
		data = report->beginTagReportData();
		data != report->endTagReportData();
		data++
	) {

		uint64_t readerTime = (*data)
			->getFirstSeenTimestampUTC()
			->getMicroseconds();
		
		uint64_t localTime = getCurrenTimeInMicroseconds();

		char * rfid = (char *) malloc(sizeof(char) * 64);
		CParameter * parameter = (*data)->getEPCParameter();
		formatOneEPC(parameter, rfid, 64);

		uint16_t antenna = (*data)->getAntennaID()
			? (*data)->getAntennaID()->getAntennaID() : 0;
		
		int8_t rssi = (*data)->getPeakRSSI()
			? (*data)->getPeakRSSI()->getPeakRSSI() : 0;

		Reading * reading = reading_create(
			readerTime, localTime, 
			antenna, rfid, rssi
		);
		
		readings_add(reading);
	}
}

void receiveAccessReports(
		CConnection * connection, 
		int duration, 
		int timeout,
		void (* accessReportHandler) (CRO_ACCESS_REPORT *)
	) {
    int done = 0;
    time_t startTime = time(NULL);
    time_t tempTime;
    
    const CTypeDescriptor * messageType;

    // keep receiving messages until done or until something bad happens
    while (!done) {
        CMessage * message = recvMessage(connection, timeout);

		// validate the timestamp
        tempTime = time(NULL);
        if (difftime(tempTime, startTime) > duration) done = 1;
        
		// make sure we received a message
        if (NULL == message) continue;

        messageType = message->m_pType;

        // handle messages by checking their types
        if (&CRO_ACCESS_REPORT::s_typeDescriptor == messageType) {
            CRO_ACCESS_REPORT * report = (CRO_ACCESS_REPORT * ) message;
			accessReportHandler(report);
		// TODO: handle various types of messages (e.g. antenna events)
        } else {
            printf("[WARN] Unexpected message\n");
        }

        delete message;
    }
}

