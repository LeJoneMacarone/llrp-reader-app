#include "readings-buffer.h"

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t full, empty;
pthread_mutex_t mutex;

size_t readingsBufferSize;
Reading ** readings;

int insertIndex = 0,
	removeIndex = 0,
	readingsCount = 0;

void readings_add(Reading * reading) {
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);
	
	readings[insertIndex] = reading;
	insertIndex = (insertIndex + 1) % readingsBufferSize;
	readingsCount++;
	
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
}

Reading * readings_take() {
	if (readingsCount <= 0) return NULL;
	
	sem_wait(&full);
	pthread_mutex_lock(&mutex);
	
	Reading * reading = readings[removeIndex];
	removeIndex = (removeIndex + 1) % readingsBufferSize;
	readingsCount--;

	pthread_mutex_unlock(&mutex);
	sem_post(&empty);

	return reading;
}

void readings_init(size_t size) {
	readingsBufferSize = size;
	readings = (Reading **) malloc(size * sizeof(Reading *));
	memset(readings, 0, size * sizeof(Reading *));

	sem_init(&empty, 0, size);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&mutex, NULL);
}

int readings_count() {
	return readingsCount;
}

cJSON * readings_toJSON() {
	cJSON * json = cJSON_CreateArray();
	for (size_t i = 0; i < readingsBufferSize; i++) {
		if (!readings[i]) continue;
		cJSON * reading = reading_toJSON(readings[i]);
		cJSON_AddItemToArray(json, reading);
	}
	return json;
}

char * readings_toString() {
	cJSON * json = readings_toJSON();
	char * string = cJSON_Print(json);
	cJSON_Delete(json);
	return string;
}
