#include "../../lib/ltkcpp/ltkcpp.h"
#include "../../lib/ltkcpp/impinj_ltkcpp.h"
using namespace LLRP;

/*
 * Enables Impinj extensions on the specified connection, by sending a 
 * command to the reader and verifies the response to ensure the extensions 
 * are successfully enabled.
 */
int enableImpinjExtensions(CConnection * connection);

/*
 * TODO: add documentation
 */
int resetConfigurationToFactoryDefaults(CConnection * connection);
