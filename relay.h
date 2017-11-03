#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


/* Each shared memory block = 1024 bytes */
#define SHM_SIZE 1024 
#define MAX_BUFFER 100


typedef struct shared_message {
	long long message_id;
	char message_buffer[ 1016 ];
}xMessage;




