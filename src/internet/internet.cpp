#include "internet.h"

size_t writeToString(void* contents, size_t size, size_t nmemb, string* buffer) {
    size_t newLength = size*nmemb;
    buffer->append((char*) contents, newLength);
    return newLength;
}

void fetchData(const char *URL, string* buffer){
	CURL *curl;
	CURLcode res;
 
	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	curl = curl_easy_init();
	if(!curl) return;
	curl_easy_setopt(curl, CURLOPT_URL, URL);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

	/* Perform the request, res gets the return code */
	res = curl_easy_perform(curl);
	
	/* Check for errors */
  	if(res != CURLE_OK)
		cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";

  	/* always cleanup */
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}
