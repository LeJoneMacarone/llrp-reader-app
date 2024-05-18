#include "internet/internet.h"
#include "logs/logs.h"
#include "reader/reader.h"

int main (int argc, char *argv[]) {
	ReaderClientConfig * config = fileToConfig(argv[1]);
	readerClientRun(config);
}
