#include "reader-utils.h"

CMessage * transact(CConnection * connection, CMessage * request)
{
	// TODO: log the requested message

    CMessage * response = connection->transact(request, 5000);
	
	// TODO: change printfs to logs

    if (NULL == response)
    {
        const CErrorDetails *   pError = connection->getTransactError();

        printf(
			"ERROR: %s transact failed, %s\n",
            request->m_pType->m_pName,
            pError->m_pWhatStr ? pError->m_pWhatStr : "no reason given"
		);

        if (NULL != pError->m_pRefType)
        {
            printf(
				"ERROR: ... reference type %s\n",
                pError->m_pRefType->m_pName
			);
        }

        if (NULL != pError->m_pRefField)
        {
            printf(
				"ERROR: ... reference field %s\n",
                pError->m_pRefField->m_pName
			);
        }

        return NULL;
    }
	
	// TODO: log response message
   
	// in case of being an error message
    if (&CERROR_MESSAGE::s_typeDescriptor == response->m_pType)
    {
        const CTypeDescriptor * pResponseType;

        pResponseType = request->m_pType->m_pResponseType;

        printf(
			"ERROR: Received ERROR_MESSAGE instead of %s\n",
            pResponseType->m_pName
		);
        
		delete response;
        response = NULL;
    }

    return response;
}

int sendMessage(CConnection * connection, CMessage * message) {
	// TODO: change printfs to logs;

	if (RC_OK != connection->sendMessage(message))
    {
        const CErrorDetails * error = connection->getSendError();

        printf(
			"ERROR: %s sendMessage failed, %s\n",
            message->m_pType->m_pName,
            error->m_pWhatStr ? error->m_pWhatStr : "no reason given"
		);

        if (NULL != error->m_pRefType)
        {
            printf(
				"ERROR: ... reference type %s\n",
                error->m_pRefType->m_pName
			);
        }

        if (NULL != error->m_pRefField)
        {
            printf(
				"ERROR: ... reference field %s\n",
                error->m_pRefField->m_pName
			);
        }

        return -1;
    }

    return 0;
}

CMessage * recvMessage(CConnection * connection, int timeoutMS) {
    // TODO: change printfs to logs

	CMessage * message = connection->recvMessage(timeoutMS);

    if (NULL == message)
    {
        const CErrorDetails * error = connection->getRecvError();

        if (error->m_eResultCode != RC_RecvTimeout)
        {
        	printf(
				"ERROR: recvMessage failed, %s\n",
				error->m_pWhatStr ? error->m_pWhatStr : "no reason given"
			);
        }

        if (NULL != error->m_pRefType)
        {
            printf(
				"ERROR: ... reference type %s\n",
                error->m_pRefType->m_pName
			);
        }

        if (NULL != error->m_pRefField)
        {
            printf(
				"ERROR: ... reference field %s\n",
                error->m_pRefField->m_pName
			);
        }

        return NULL;
    }

	// TODO: log the connection message 

    return message;
}

int checkLLRPStatus (CLLRPStatus * pLLRPStatus, const char * pWhatStr) {
    /*
     * The LLRPStatus parameter is mandatory in all responses.
     * If it is missing there should have been a decode error.
     * This just makes sure (remember, this program is a
     * diagnostic and suppose to catch LTKC mistakes).
     */
    
    if(NULL == pLLRPStatus)
    {
        printf("ERROR: %s missing LLRP status\n", pWhatStr);
        return -1;
    }

    /*
     * Make sure the status is M_Success.
     * If it isn't, print the error string if one.
     * This does not try to pretty-print the status
     * code. To get that, run this program with -vv
     * and examine the XML output.
     */
    if(StatusCode_M_Success != pLLRPStatus->getStatusCode())
    {
        llrp_utf8v_t            ErrorDesc;

        ErrorDesc = pLLRPStatus->getErrorDescription();

        if(0 == ErrorDesc.m_nValue)
        {
            printf("ERROR: %s failed, no error description given\n",
                pWhatStr);
        }
        else
        {
            printf("ERROR: %s failed, %.*s\n",
                pWhatStr, ErrorDesc.m_nValue, ErrorDesc.m_pValue);
        }
        return -2;
    }

    /*
     * Victory. Everything is fine.
     */
    return 0;
}

void printTagReportData(CRO_ACCESS_REPORT * accessReport) {
    std::list < CTagReportData * > ::iterator current;
    unsigned int nEntries = 0;

    // count the number of entries
    for (
        current = accessReport->beginTagReportData(); 
        current != accessReport->endTagReportData(); 
        current++
    ) {
        nEntries++;
    }

    // TODO: log the number of entries
    printf("INFO: number of entries: %i\n", nEntries);

    // print each entry
    for (
        current = accessReport->beginTagReportData(); 
        current != accessReport->endTagReportData(); 
        current++
    ) {
        printOneTagReportData(*current);
    }
}

void printOneTagReportData(CTagReportData * pTagReportData) {
    char buffer[64];

    CParameter * epcParameter = pTagReportData->getEPCParameter();

    formatOneEPC(epcParameter, buffer, 64);

    // TODO: log the EPC
    printf("EPC: %s\n", buffer);
}

void printXMLMessage(CMessage * message) {
    char buffer[100 * 1024];

    message->toXMLString(buffer, sizeof(buffer));
    
    // TODO: log the content
    printf("%s\n", buffer);
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
