#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

/*
 * Adds a ROSpec to the reader. Equivalent to transacting an ADD_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int addROSpec(CConnection * connection);

/*
 * Enables a ROSpec on the reader. Equivalent to transact an ENABLE_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int enableROSpec(CConnection * connection);

/*
 * Starts a ROSpec on the reader. Equivalent to transact an START_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int startROSpec(CConnection * connection);

/*
 * Stops a ROSpec on the reader. Equivalent to transact an STOP_ROSPEC 
 * message. Returns the reponse or NULL in case of an error. 
 */
int stopROSpec(CConnection * connection);
