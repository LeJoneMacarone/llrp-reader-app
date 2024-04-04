#include <curl/curl.h>
#include <iostream>

using namespace std;

size_t writeToString(void *contents, size_t size, size_t nmemb, char** buffer)
{
    size_t newLength = size*nmemb;
    
	*buffer = new char[newLength];
	
	return newLength;
}

void fetchData(const char *URL, const char** buffer){
	CURL *curl;
	CURLcode res;
 
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	curl = curl_easy_init();
	if(!curl) return;
	curl_easy_setopt(curl, CURLOPT_URL, URL);

	/* cache the CA cert bundle in memory for a week */
	curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

	/* Perform the request, res gets the return code */
	res = curl_easy_perform(curl);
	
	/* Check for errors */
  	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

  	/* always cleanup */
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}
