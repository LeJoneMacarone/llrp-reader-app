#include "../../lib/ltkcpp/ltkcpp.h"
using namespace LLRP;

/*
 * Adds a ROSpec to the reader. Equivalent to transacting an ADD_ROSPEC 
 * message. Returns 0 if successfull or -1 in case of error. 
 */
int addROSpec(CConnection * connection);

/*
 * Enables a ROSpec on the reader. Equivalent to transact an ENABLE_ROSPEC 
 * message. Returns 0 if successfull or -1 in case of error. 
 */
int enableROSpec(CConnection * connection);

/*
 * Starts a ROSpec on the reader. Equivalent to transact an START_ROSPEC 
 * message. Returns 0 if successfull or -1 in case of error. 
 */
int startROSpec(CConnection * connection);

/*
 * Stops a ROSpec on the reader. Equivalent to transact an STOP_ROSPEC 
 * message. Returns 0 if successfull or -1 in case of error. 
 */
int stopROSpec(CConnection * connection);
