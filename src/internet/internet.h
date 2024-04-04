#include <curl/curl.h>
#include <iostream>

using namespace std;

/*
 * Fetches data from the specified URL.
 */
void fetchData(const char* URL, string* buffer);
