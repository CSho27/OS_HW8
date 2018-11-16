/* Chris Fietkiewicz. Optional example of creating different types of
   shared memory with a single function.
*/
#include <unistd.h> // For shared memory
#include <stdio.h> // For printf
#include <stdlib.h> // For exit
#include <fcntl.h> // For shared memory
#include <sys/shm.h> // For shared memory
#include <sys/mman.h> // For shared memory
#include <semaphore.h> // For semaphores
#include <string.h> // For string operations

#define BUFLEN 1024
#define MINI_BUFLEN 32

// Create shared memory
void* create_shm(const char* name, int SIZE) {
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	void *memory = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (memory == MAP_FAILED) {
		printf("Map failed\n");
		exit(0);
	}
	return memory;
}

int main() {
	// Set up shared memory for string
	char* buffer = "buffer";
	char* string = (char*) create_shm(buffer, MINI_BUFLEN);
	sprintf(string, "initial string");
	
	// Set up shared memory for semaphore
	const char *mutex_read_name = "mutex_read";
	sem_t *mutex_read = (sem_t*) create_shm(mutex_read_name, 1);
	if(sem_init(mutex_read, 1, 1)) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	// Set up shared memory for semaphore
	const char *mutex_write_name = "mutex_write";
	sem_t *mutex_write = (sem_t*) create_shm(mutex_write_name, 1);
	if(sem_init(mutex_write, 1, 1)) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	// Set up shared memory for semaphore
	const char *read_try_name = "read_try";
	sem_t *read_try = (sem_t*) create_shm(read_try_name, 1);
	if(sem_init(read_try, 1, 1)) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	// Set up shared memory for semaphore
	const char *resource_name = "resource";
	sem_t *resource = (sem_t*) create_shm(resource_name, 1);
	if(sem_init(resource, 1, 1)) { // 1 = multiprocess
		fprintf(stderr, "ERROR: could not initialize semaphore.\n");
		exit(0);
	}

	// Set up shared memory for integer
	const char *read_count_name = "read_count";
	create_shm(read_count_name, 1);

	// Set up shared memory for integer
	const char *write_count_name = "write_count";
	create_shm(write_count_name, 1);

	const char *version_name = "version";
	create_shm(version_name, 1);

	printf("Press ENTER to quit and unlink shared memory\n");
	char blank[MINI_BUFLEN];
	fgets(blank, MINI_BUFLEN, stdin);

	// Remove shared memory segments
	if (shm_unlink(buffer) == -1 || shm_unlink(mutex_read_name) == -1 || shm_unlink(mutex_write_name) == -1 || shm_unlink(read_try_name) == -1 || shm_unlink(resource_name) == -1 ||
			shm_unlink(read_count_name) == -1 || shm_unlink(write_count_name) == -1 || shm_unlink(version_name) == -1) {
		printf("Error removing shared memory\n");
		exit(-1);
	}
	else{
		printf("Unlinking shared memory\n");
	}
	return 0;
}
