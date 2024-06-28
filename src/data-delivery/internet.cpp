#include "internet.h"
#include <curl/curl.h>
#include <curl/easy.h>

size_t writeToString(void* contents, size_t size, size_t nmemb, string* buffer) {
    size_t newLength = size*nmemb;
    buffer->append((char*) contents, newLength);
    return newLength;
}

long performRequest(const char *url, string* buffer){
	CURL * curl;
	CURLcode res;
 
	// initialize curl
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (!curl) return -1;

	// setup curl request
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

	// perform request
	res = curl_easy_perform(curl);
  	if (res != CURLE_OK) return -1;

	// get status code
	long status;
	res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

  	if (res != CURLE_OK) return -1;

	// cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return status;
}
