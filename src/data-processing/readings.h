typedef struct {
    char * time; 
	unsigned int antenna;	
    char * rfid; 
    int rssi; 
} Reading;

Reading * reading_create(
	const char * time,	
	unsigned int antenna, 
	const char * rfid,
	int rssi
);

char * reading_toJsonString(Reading * reading);

void readings_add(Reading * reading);

void readings_print();
