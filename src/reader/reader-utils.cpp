#include "reader-utils.h"
#include "../data/readings.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <list>

using namespace std;

void printTagReportData(CRO_ACCESS_REPORT * accessReport) {
    list<CTagReportData *>::iterator current;
    unsigned int nEntries = 0;
    
	// count the number of entries
    for (
        current = accessReport->beginTagReportData(); 
        current != accessReport->endTagReportData(); 
        current++
    ) {
        nEntries++;
    }

    printf("[INFO] number of entries: %i\n", nEntries);

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
	
	for (
		data = report->beginTagReportData();
		data != report->endTagReportData();
		data++
	) {

		#ifdef USE_READER_TIMESTAMPS
			uint64_t timestamp = (*data)->getFirstSeenTimestampUTC()->getMicroseconds(); 
		#else
			time_t timestamp = time(NULL);
		#endif
		
		char rfid[64];
		CParameter * parameter = (*data)->getEPCParameter();
		formatOneEPC(parameter, rfid, 64);
	
		uint16_t id = (*data)->getAntennaID()
			? (*data)->getAntennaID()->getAntennaID() : 0;
		
		int8_t rssi = (*data)->getPeakRSSI()
			? (*data)->getPeakRSSI()->getPeakRSSI() : 0;

		Reading * reading = reading_create(
			"2020", "golemu", "localhost", 
			id, 
			rfid,
			rssi, 
			"123 - Pedro Alves", 
			0
		);
		
		printf("[INFO] Reading: %s\n", reading_toJsonString(reading));
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

    if(NULL != pEPCParameter)
    {
        llrp_u96_t my_u96;
        llrp_u1v_t my_u1v;
        llrp_u8_t * pValue = NULL;
        
		unsigned int n, i;
		
        const CTypeDescriptor * pType;
        pType = pEPCParameter->m_pType;
        if(&CEPC_96::s_typeDescriptor == pType)
        {
            CEPC_96 * pEPC_96;

            pEPC_96 = (CEPC_96 *) pEPCParameter;
            my_u96 = pEPC_96->getEPC();
            pValue = my_u96.m_aValue;
            n = 12u;
        }
        else if(&CEPCData::s_typeDescriptor == pType)
        {
            CEPCData * pEPCData;

            pEPCData = (CEPCData *) pEPCParameter;
            my_u1v = pEPCData->getEPC();
            pValue = my_u1v.m_pValue;
            n = (my_u1v.m_nBit + 7u) / 8u;
		}

        if(NULL != pValue)
        {
            for(i = 0; i < n; i++)
            {
                if(0 < i && i%2 == 0 && 1 < bufsize)
                {
                    *p++ = '-';
                    bufsize--;
                }
                if(bufsize > 2)
                {
                    written = snprintf(p, bufsize, "%02X", pValue[i]);
                    bufsize -= written;
                    p+= written;
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

    // null terminate this for good practice
    buf[buflen-1] = '\0';
}
