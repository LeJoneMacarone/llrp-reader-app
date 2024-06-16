#include "readings-buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t full, empty;
pthread_mutex_t mutex;

#define READINGS_BUFFER_SIZE 256
Reading * readings[READINGS_BUFFER_SIZE];

int insertIndex = 0,
	removeIndex = 0,
	readingsCount = 0;

void readings_add(Reading * reading) {
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);
	
	readings[insertIndex] = reading;
	insertIndex = (insertIndex + 1) % READINGS_BUFFER_SIZE;
	readingsCount++;
	
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
}

Reading * readings_take() {
	if (readingsCount <= 0) return NULL;

	sem_wait(&full);
	pthread_mutex_lock(&mutex);
	
	Reading * reading = readings[removeIndex];
	readings[removeIndex] = NULL;
	removeIndex = (removeIndex + 1) % READINGS_BUFFER_SIZE;
	readingsCount--;
	
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	return reading;
}

void readings_init() {
	sem_init(&empty, 0, READINGS_BUFFER_SIZE);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&mutex, NULL);
}

int readings_count() {
	return readingsCount;
}

void readings_print(){
	printf("[INFO] readings count: %i\n", readingsCount);

	for (int i = 0; i < READINGS_BUFFER_SIZE; i++) {
		if (readings[i] == NULL) continue;
		printf("[INFO] Reading[%i] = %s\n", i, reading_toString(readings[i]));
	}
}
