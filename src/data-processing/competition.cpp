#include "competition.h"

#include <stdlib.h>
#include <string.h>

Competition * competition_create(
	const char * name, 
	uint64_t start, 
	uint64_t end, 
	const char * color
) {
    Competition * competition = (Competition *) malloc(sizeof(Competition));

    competition->name = strdup(name);
    competition->start = start;
    competition->end = end;
    competition->color = strdup(color);

    return competition;
}

Competition * competition_fromJSON(cJSON * json) {
	char * name = cJSON_GetObjectItem(json, "short_name")->valuestring;
	char * start = cJSON_GetObjectItem(json, "start")->valuestring;
	char * end = cJSON_GetObjectItem(json, "start")->valuestring;
	char * color = cJSON_GetObjectItem(json, "color")->valuestring;
	return NULL;
}
