//Christopher Shorter
//OS_HW8
//Problem 2
#include <stdio.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <stdbool.h>

#define BUFLEN 1024
#define MINI_BUFLEN 32

void* open_shm(char *name){
	const int SIZE = 4096;
	int shm_fd = shm_open(name, O_RDWR, 0666);
    if(shm_fd ==-1){
        printf("shared memory failed \n");
        exit(-1);
    }
	
	void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        exit(-1);
    }
	return ptr;
}



int main(){
	bool done = false;
	char* buffer = (char*) open_shm("buffer");
	sem_t* mutex_write = (sem_t*) open_shm("mutex_write");
	sem_t* read_try = (sem_t*) open_shm("read_try");
	sem_t* resource = (sem_t*) open_shm("resource");
	int* read_count = (int*) open_shm("read_count");
	int* write_count = (int*) open_shm("write_count");

	do{
		printf("Enter the message you would like to write to the shared string or type -q to quit:\n");
		char read_buf[MINI_BUFLEN];
		fgets(read_buf, MINI_BUFLEN, stdin);
		if(read_buf[0] == '-'){
			if(read_buf[1] == 'q')
				done = true;
		}
		read_buf[MINI_BUFLEN-1] = '\0';

		sem_wait(mutex_write);
		*write_count = *write_count+1;
		if(*write_count == 1){
			sem_wait(read_try);
		}
		if(*read_count > 0)
			printf("read_count = %d\n", *read_count);
		sem_post(mutex_write);

		sem_wait(resource);
		bzero(buffer, MINI_BUFLEN);
		sprintf(buffer, "%s", read_buf);
		sem_post(resource);

		sem_wait(mutex_write);
		*write_count = *write_count-1;
		if(*write_count == 0)
			sem_post(read_try);
		sem_post(mutex_write);
	}while(!done);
	
	return 0;
}
