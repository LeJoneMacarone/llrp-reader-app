#include "config/config.h"
#include "internet/internet.h"

int main (int argc, char *argv[]) {
	const char* configFile = argv[1];
	
	Configuration* config = fileToConfig(configFile);

	if (!config) cout << "An error has occured\n";
	
	cout << "server-url = " << config->serverURL << "\n";

	string URL;
 	const char* buffer;

	URL.append(config->serverURL);
	URL.append("greninja");

	fetchData(URL.c_str(), &buffer);
	cout << buffer;

	// comment
}
