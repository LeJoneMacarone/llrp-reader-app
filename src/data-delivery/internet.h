#include <string>
using namespace std;

/*
 * Executes a request from on specified URL, filling the buffer with the 
 * given response. Returns the status code of the request or -1 in case of 
 * error.
 */
long performRequest(const char * URL, string * buffer);
