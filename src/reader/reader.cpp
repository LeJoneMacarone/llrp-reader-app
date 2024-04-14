#include "reader.h"

#define KB 1024u

CConnection* connectToReader(const char* hostname) {
 	// the registry is used for decoding messages
 	CTypeRegistry* typeRegistry = getTheTypeRegistry();
 	enrollCoreTypesIntoRegistry(typeRegistry);
 
 	// initialize a connection object with the type registry 
 	// and 32KB of max frame size for send/recv
 	CConnection* connection = new CConnection(typeRegistry, 32u*KB);
 
 	if (NULL == connection) {
 		printf("ERROR: new CConnection failed\n");
 		return NULL;
 	}
 	
 	// open the connection with the provided hostname
 	int status = connection->openConnectionToReader(hostname);
 	
	if (0 != status) {
 		printf("ERROR: connection failed: %s (%i)\n", 
 			connection->getConnectError(), 
 			status
 		);
 		delete connection;
 		return NULL;
 	}
 
 	return connection;
 }

/* 
 * Converts a CEPCData to a string. Internally, is converting 
 * an array of bits to an array of characters.
 *
 * TODO: provide references for the conversion algorithm  
 */
void formatOneEPC (
	CParameter *pEPCParameter, char *buf, int buflen
) {
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
