#include "config/config.h"
#include "internet/internet.h"
#include <string>

int main (int argc, char *argv[]) {
	const char* configFile = argv[1];

	Configuration* config = fileToConfig(configFile);

	if (!config) cerr << "fileToConfig: an error has occured while loading the configuration\n";
		
	cout << "server-url = " << config->serverURL << "\n";

 	string buffer;

	fetchData(config->serverURL, &buffer);
	
	cout << buffer << "\n";

	// comment
}
