#include "logs.h"
#include <stdarg.h>

const char * flagString[] = { 
	"[INFO] ", 
	"[WARN] ",
	"[ERROR] ",
	"[DEBUG] ",
};

void log(FILE * stream, logflag flag, const char * format, ...) {
	va_list args;
	va_start(args, format);

	fputs(flagString[flag], stream);
	vfprintf(stream, format, args);
	fputs("\n", stream);
	
	va_end(args);
}
