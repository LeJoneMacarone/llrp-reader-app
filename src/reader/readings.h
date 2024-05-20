typedef struct {
    char * time; // "2023-12-02 09:00:46.321",
    char * source; // "Leitor R420 @ 192.168.1.62 #1",
    char * rfid; // "00330001000004a6",
    int rssi; // "--67",
    char * athlete; // " - ",
    int crossing; // true
} Reading;

Reading * reading_create(
	const char * time,
	const char * reader_name,
	const char * host,
	unsigned int antenna,
	const char * rfid,
	int rssi,
	const char * athlete,
	int crossing
);
