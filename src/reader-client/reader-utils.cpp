#include "reader-utils.h"
#include "../data-processing/readings.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <list>
using namespace std;

#define USE_READER_TIMESTAMPS

void printTagReportData(CRO_ACCESS_REPORT * accessReport) {
    list<CTagReportData *>::iterator current;

    printf(
			"[INFO] number of entries: %i\n", 
			accessReport->countTagReportData()
	);

    // print each entry
    for (
        current = accessReport->beginTagReportData(); 
        current != accessReport->endTagReportData(); 
        current++
    ) {
        printOneTagReportData(*current);
    }
}

void saveAccessReport(CRO_ACCESS_REPORT * report) {
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

		#ifdef USE_READER_TIMESTAMPS
			uint64_t timestamp = (*data)
				->getFirstSeenTimestampUTC()
				->getMicroseconds();
		#else
			time_t timestamp = time(NULL);
		#endif
		
		char * rfid = (char *) malloc(sizeof(char) * 64);
		CParameter * parameter = (*data)->getEPCParameter();
		formatOneEPC(parameter, rfid, 64);

		printf("[INFO] EPC: %s\n", rfid);

		uint16_t antenna = (*data)->getAntennaID()
			? (*data)->getAntennaID()->getAntennaID() : 0;
		
		int8_t rssi = (*data)->getPeakRSSI()
			? (*data)->getPeakRSSI()->getPeakRSSI() : 0;

		Reading * reading = reading_create("2020", antenna, rfid, rssi);
		
		readings_add(reading);
	}
}

void printOneTagReportData(CTagReportData * pTagReportData) {
    char buffer[64];

    CParameter * epcParameter = pTagReportData->getEPCParameter();

    formatOneEPC(epcParameter, buffer, 64);

	uint64_t timestamp = pTagReportData->getFirstSeenTimestampUTC() 
		? pTagReportData->getFirstSeenTimestampUTC()->getMicroseconds()	: 0; 

	printf("[INFO] [%lu] EPC: %s\n", timestamp, buffer);
}

void printXMLMessage(CMessage * message) {
    char buffer[100 * 1024];

    message->toXMLString(buffer, sizeof(buffer));
    
    printf("[INFO] %s\n", buffer);
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
        }
        else
        {
            written = snprintf(p, bufsize, "%s", "---unknown-epc-data-type---");
            bufsize -= written;
            p += written;
        }
    }
    else
    {
        written = snprintf(p, bufsize, "%s", "--null epc---");
        bufsize -= written;
        p += written;
    }

    buf[buflen-1] = '\0';
}
