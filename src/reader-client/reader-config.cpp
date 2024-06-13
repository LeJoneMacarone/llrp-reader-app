#include "reader-config.h"
#include "reader-messages.h"

int enableImpinjExtensions(CConnection * connection) {
    // compose the command message
    CIMPINJ_ENABLE_EXTENSIONS * command = new CIMPINJ_ENABLE_EXTENSIONS();
    command->setMessageID(getMessageID());
    
    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received something
    if (NULL == responseMessage) return -1;

    CIMPINJ_ENABLE_EXTENSIONS_RESPONSE * response 
        = (CIMPINJ_ENABLE_EXTENSIONS_RESPONSE *) responseMessage;

    // Check the LLRPStatus parameter.
    if (0 != checkLLRPStatus(
        response->getLLRPStatus(), 
        "enableImpinjExtensions"
    )) {
        delete responseMessage;
        return -1;
    }

    delete responseMessage;

    return 0;
}

int resetConfigurationToFactoryDefaults(CConnection * connection) {
    // compose the command message
    CSET_READER_CONFIG * command;
    command = new CSET_READER_CONFIG();
    command->setMessageID(getMessageID());
    command->setResetToFactoryDefault(1);

    // send/recv message
    CMessage * responseMessage = transact(connection, command);

    delete command;

    // make sure we received a message
    if (NULL == responseMessage) return -1;

    CSET_READER_CONFIG_RESPONSE * response 
        = (CSET_READER_CONFIG_RESPONSE * ) responseMessage;
    
    // check the LLRPStatus parameter
    if (0 != checkLLRPStatus(
        response->getLLRPStatus(),
        "resetConfigurationToFactoryDefaults"
    )) {
        /* checkLLRPStatus already tattled */
        delete responseMessage;
        return -1;
    }

    delete responseMessage;
    
    return 0;
}

