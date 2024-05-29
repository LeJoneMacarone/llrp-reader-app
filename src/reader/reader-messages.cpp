#include "reader-messages.h"

#include <stdio.h>

CMessage * transact(CConnection * connection, CMessage * request)
{
    CMessage * response = connection->transact(request, 5000);

    if (NULL == response) {
		printf(
			"[ERROR] transact failed for message %s\n",
			request->m_pType->m_pName
		);
        return NULL;
    }
	
	// In case of being an error message
    if (&CERROR_MESSAGE::s_typeDescriptor == response->m_pType) {
        const char * responseType = request->m_pType->m_pResponseType->m_pName;

       	printf("[ERROR] received ERROR_MESSAGE instead of %s\n", responseType);

		delete response;

        response = NULL;
    }

    return response;
}

int sendMessage(CConnection * connection, CMessage * message) {
	if (RC_OK == connection->sendMessage(message)) return 0;

	printf(
		"[ERROR] send messsage failed for message %s\n", 
		message->m_pType->m_pName
	);

    return -1;
}

CMessage * recvMessage(CConnection * connection, int timeoutMS) {
	CMessage * message = connection->recvMessage(timeoutMS);

    if (NULL == message) {
        const CErrorDetails * error = connection->getRecvError();

        if (error->m_eResultCode != RC_RecvTimeout) {
			printf(
				"[ERROR] receive message failed, %s\n",
				error->m_pWhatStr ? error->m_pWhatStr : "no reason given"
			);
        }

        return NULL;
    }

	printf("[INFO] received message %s\n", message->m_pType->m_pName);

    return message;
}

int checkLLRPStatus (CLLRPStatus * pLLRPStatus, const char * pWhatStr) {
    // The LLRPStatus parameter is mandatory in all responses.
    // If it is missing there should have been a decode error.
    if (NULL == pLLRPStatus) {
        printf("[ERROR] %s missing LLRP status\n", pWhatStr);
        return -1;
    }

    // Make sure the status is M_Success.
    // If it isn't, print the error string if one.
    if(StatusCode_M_Success != pLLRPStatus->getStatusCode())
    {
        llrp_utf8v_t ErrorDesc = pLLRPStatus->getErrorDescription();

        if (0 == ErrorDesc.m_nValue) {
			printf(
				"[ERROR] %s failed, no error description given\n", 
				pWhatStr
			);
		} else {
            printf(
				"[ERROR] %s failed, %.*s\n",
				pWhatStr, 
				ErrorDesc.m_nValue, 
				ErrorDesc.m_pValue
			);
        }
        return -2;
    }

    return 0;
}

