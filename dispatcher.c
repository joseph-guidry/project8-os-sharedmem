#include "relay.h"

int main(void)
{
	key_t key;
	int shmid;
	xMessage *data;
	long long msgid = 1;
	char buffer[MAX_BUFFER];

	

	/* Make a key */
	if ( (key = ftok("/", 'A')) == -1 )
	{
		perror("ftok");
		exit(1);
	}

	/* connect to/create segment */
	if ( (shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1 )
	{
		perror("shmget");
		exit(2);
	}

	/* attach to the segment to a pointer to it */
	data = shmat(shmid, (void *) 0, 0);
	if ( data == (xMessage *)(-1))
	{
		perror("shmat");
		exit(3);
	}
	
	data->message_id = 0;

	while ( fgets(buffer, MAX_BUFFER, stdin) != NULL )
	{

		if (feof(stdin) )
			break;

		/* replace \n with a NULL terminator */
		buffer[strlen(buffer) - 1] = '\0';
		strncpy(data->message_buffer, buffer, sizeof(buffer));
		data->message_id = msgid++;

	/*
		printf("message # [%lld] \n", data->message_id);
		printf("message [%s] \n", data->message_buffer);
	*/
	}

	/* Set the exit value for message_id in the data segment */
	data->message_id = -1;
	printf("updated message id to -1? [%lld]\n", data->message_id);

	if (shmdt(data) == -1 )
	{
		perror("shmdt");
		exit(4);
	}
	/* Wait for 2 seconds to allow clients to read signal */
	printf("Disconnecting clients ...\n");
	sleep(1);
	shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
