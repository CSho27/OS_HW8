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
	sem_t* rw_mutex = (sem_t*) open_shm("mutex_rw");
	int* version = (int*) open_shm("version");

	do{
		printf("Enter the message you would like to write to the shared string or type -q to quit:\n");
		char read_buf[BUFLEN];
		fgets(read_buf, BUFLEN, stdin);
		if(read_buf[0] == '-'){
			if(read_buf[1] == 'q')
				done = true;
		}
		read_buf[BUFLEN-1] = '\0';

		sem_wait(rw_mutex);
		bzero(buffer, BUFLEN);
		sprintf(buffer, "%s", read_buf);
		version++;
		sem_post(rw_mutex);
	}while(!done);
	
	return 0;
}
