#include "reader.h"
#include <cstddef>

#define KB 1024u

CConnection* connectToReader(const char* hostname) {
 	// TODO: change printfs to logs

	// the registry is used for decoding messages
 	CTypeRegistry* typeRegistry = getTheTypeRegistry();
 	enrollCoreTypesIntoRegistry(typeRegistry);
 
 	// initialize a connection object with the type registry and 32KB of max
	// frame size for send/recv
 	CConnection* connection = new CConnection(typeRegistry, 32u*KB);
 
 	if (NULL == connection) {
 		printf("ERROR: new CConnection failed\n");
 		return NULL;
 	}
 	
 	// open the connection with the provided hostname
 	int status = connection->openConnectionToReader(hostname);
 	
	if (0 != status) {
 		printf(
			"ERROR: connection failed: %s (%i)\n", 
			connection->getConnectError(), 
 			status
 		);
 		delete connection;
 		return NULL;
 	}
 
 	return connection;
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

/* 
 * Converts a CEPCData to a string. Equivalent to converting an array of bits 
 * to an array of characters.
 *
 * TODO: provide references for the conversion algorithm  
 */
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

CMessage * addROSpec(CConnection * connection) {
	// initialize fields and parameters of the ROSpec, from the most internal 
	// to the the most external
	
	// ROSpecStartTrigger
	CROSpecStartTrigger * startTrigger = new CROSpecStartTrigger();
	startTrigger->setROSpecStartTriggerType(ROSpecStartTriggerType_Null);
	
	// ROSpecStopTrigger
	CROSpecStopTrigger * stopTrigger = new CROSpecStopTrigger();
	stopTrigger->setROSpecStopTriggerType(ROSpecStopTriggerType_Null);
	stopTrigger->setDurationTriggerValue(0);

	// ROBoundarySpec = (ROSpecStartTrigger, ROSpecStopTrigger)
	CROBoundarySpec * boundarySpec = new CROBoundarySpec();
	boundarySpec->setROSpecStartTrigger(startTrigger);
	boundarySpec->setROSpecStopTrigger(stopTrigger);
	
	// AISpecStopTrigger
	CAISpecStopTrigger * aiSpecStopTrigger = new CAISpecStopTrigger();
	aiSpecStopTrigger->setAISpecStopTriggerType(AISpecStopTriggerType_Null);
	aiSpecStopTrigger->setDurationTrigger(0);

	// InventoryParameterSpec 
	CInventoryParameterSpec * inventoryParameterSpec 
		= new CInventoryParameterSpec();
	inventoryParameterSpec->setInventoryParameterSpecID(1234);
	inventoryParameterSpec->setProtocolID(AirProtocols_EPCGlobalClass1Gen2);
	
	// AntennaIDs
    llrp_u16v_t antennaIDs = llrp_u16v_t(2);
    antennaIDs.m_pValue[0] = 1;
    antennaIDs.m_pValue[1] = 2;

	// AISpec = (AISpecStopTrigger, InventoryParameterSpec, AntennaIDs)
	CAISpec * aiSpec = new CAISpec();
	aiSpec->setAntennaIDs(antennaIDs);
	aiSpec->setAISpecStopTrigger(aiSpecStopTrigger);
	aiSpec->addInventoryParameterSpec(inventoryParameterSpec);

	// ROSpec = (ROSpecID, Priority, CurrentState, ROBoundarySpec, AISpec)
	CROSpec * roSpec = new CROSpec();
	roSpec->setROSpecID(1111);
	roSpec->setPriority(0);
	roSpec->setROBoundarySpec(boundarySpec);
	roSpec->setCurrentState(ROSpecState_Disabled);
	roSpec->addSpecParameter(aiSpec);

	// append the ROSpec ta a message
	CADD_ROSPEC * message = new CADD_ROSPEC();
	message->setROSpec(roSpec);
	message->setMessageID(0);

	// send the message
	CMessage * response = transact(connection, message);
	
	delete message;

	// TODO: log errors (i.e. NULL response)
	
	// TODO: log the ROSpec was added

	return response;
}

CMessage * enableROSpec(CConnection connection);
