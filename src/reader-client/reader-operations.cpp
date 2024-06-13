#include "reader-operations.h"
#include "reader-messages.h"

// TODO: add possibility to add the spec from a LTK-XML file
int addROSpec(CConnection * connection) {
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
	CADD_ROSPEC_RESPONSE * response 
		= (CADD_ROSPEC_RESPONSE *) transact(connection, message);
	
	delete message;
	
    if(NULL == response) return -1;

    if(0 != checkLLRPStatus(response->getLLRPStatus(), "addROSpec"))
        return -1;

    delete response;

    return 0;
}

int enableROSpec(CConnection * connection) {
    // compose the command message
    CENABLE_ROSPEC * command = new CENABLE_ROSPEC();
    command -> setMessageID(getMessageID());
    command -> setROSpecID(1111);

    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received a message
    if (NULL == responseMessage) return -1;

    CENABLE_ROSPEC_RESPONSE * response 
        = (CENABLE_ROSPEC_RESPONSE *) responseMessage;

    // check the LLRPStatus parameter.
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "enableROSpec"
    )) {
        delete responseMessage;
        return -1;
    }

    delete responseMessage;

    return 0;
}

int startROSpec(CConnection * connection) {
    // compose the command message
    CSTART_ROSPEC * command = new CSTART_ROSPEC();
    command -> setMessageID(getMessageID());
    command -> setROSpecID(1111);

    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received the message
    if (NULL == responseMessage) return -1;

    CSTART_ROSPEC_RESPONSE * response
        = (CSTART_ROSPEC_RESPONSE * ) responseMessage;

    // check the LLRPStatus parameter. 
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "startROSpec"
    )) {
        delete responseMessage;
        return -1;
    }

    delete responseMessage;
    
    return 0;
}

int stopROSpec(CConnection * connection) {
    // compose the command message
    CSTOP_ROSPEC * command = new CSTOP_ROSPEC();
    command -> setMessageID(getMessageID());
    command -> setROSpecID(1111);

   	// send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received the message
    if (NULL == responseMessage) return -1;

    CSTOP_ROSPEC_RESPONSE * response
        = (CSTOP_ROSPEC_RESPONSE * ) responseMessage;
    
    // check the LLRPStatus parameter. 
    if (0 != checkLLRPStatus(
        response -> getLLRPStatus(), 
        "stopROSpec"
    )) {
        delete responseMessage;
        return -1;
    }
    delete responseMessage;

    return 0;
}

