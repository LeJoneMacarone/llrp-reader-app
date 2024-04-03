#include "config/config.h"

int main (int argc, char *argv[]) {
	const char* configFile = argv[1];
	
	Configuration* config = fileToConfig(configFile);

	if (!config) cout << "An error has occured\n";

	cout << "server-url = " << config->serverURL << "\n";
}
