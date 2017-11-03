#include "relay.h"

int main(void)
{
	key_t key;
	int shmid = 0;
	xMessage  *data;
	long long msgid = -1;

	/* Make a key */
	if ( (key = ftok("/", 'A')) == -1 )
	{
		perror("ftok");
		exit(1);
	}

	/* connect to/create segment */
	if ( (shmid = shmget(key, SHM_SIZE, 0444 | IPC_CREAT)) == -1 )
	{
		perror("shmget");
		exit(2);
	}


	while ( 1 )
	{
		/* attach to the segment to a pointer to it */
		data = shmat(shmid, (void *) 0, 0);
		if ( data == (xMessage *)(-1))
		{
			perror("shmat");
			exit(3);
		}
/*
		printf("Message # : [%lld]\n", data->message_id);
		printf("Message   : [%s]\n", data->message_buffer);
		printf("msgid [%lld] \n", msgid);
*/	
		if ( (data->message_id) < 0)
		{
			/* This would mean the dispatcher has exited if value < 0 */
			printf("This is the kill signal\n");
			exit(1);
		}
		else if (msgid == -1 )
			msgid = data->message_id;
		else if ( (data->message_id - msgid) > 0 )
		{
			printf("Shared Data : [%s]\n", data->message_buffer);
			msgid = data->message_id;
		}

		// Sleep a little bit ?
		sleep(2);

		if ( shmdt(data) == -1 )
		{
			perror("shmdt");
			exit(5);
		}
	}
	
    return 0;
}
