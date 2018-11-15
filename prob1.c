//Christopher Shorter
//OS_HW8
//Problem 1
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define BUFLEN 1024

char buffer[BUFLEN];
int version = 0;

sem_t rw_mutex;
sem_t mutex;
int read_count = 0;

void* reader1();
void* reader2();

int main(){
	bzero(buffer, BUFLEN);

	sem_init(&rw_mutex, 0, 1);
	sem_init(&mutex, 0, 1);

	pthread_t rd1_tid;
	pthread_t rd2_tid;

	pthread_attr_t rd1_attr;
	pthread_attr_t rd2_attr;

	pthread_attr_init(&rd1_attr);
	pthread_attr_init(&rd2_attr);

	pthread_create(&rd1_tid, &rd1_attr, reader1, NULL);
	pthread_create(&rd2_tid, &rd2_attr, reader2, NULL);

	do{
		sem_wait(&rw_mutex);
		bzero(buffer, BUFLEN);
		sprintf(buffer, "version %d: \"string\"", version+1);
		version++;
		sem_post(&rw_mutex);
		sleep(1);
	}while(version<2);
	
	return 0;
}

void* reader1(){
	bool done = false;
	while(!done){
		sem_wait(&mutex);
		read_count++;
		if(read_count == 1)
			sem_wait(&rw_mutex);
		sem_post(&mutex);
		if(version == 1){
			printf("Reader #1: %s\n", buffer);
			fflush(stdout);
			done = true;
		}

		sem_wait(&mutex);
		read_count--;
		if(read_count == 0)
			sem_post(&rw_mutex);
	
		sem_post(&mutex);
	}
	return NULL;
}

void* reader2(){
	bool done = false;
	while(!done){
		sem_wait(&mutex);
		read_count++;
		if(read_count == 1)
			sem_wait(&rw_mutex);
		sem_post(&mutex);
		if(version == 2){
			printf("Reader #2: %s\n", buffer);
			fflush(stdout);
			done = true;
		}

		sem_wait(&mutex);
		read_count--;
		if(read_count == 0)
			sem_post(&rw_mutex);
		sem_post(&mutex);
	}
	return NULL;
}
