#include <signal.h>

#include "relay.h"

// Allow CTRL + C to start terminating the clients
/* Prototypes for Signal Handling */
int set_signal_handler(void);
void signal_handler (int sig);

/* The msgid is used to signal clients when dispatcher has exited */
static long msgid = 1;

int main(void)
{
	key_t key;
	int shmid;
	xMessage *data;
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
	if (set_signal_handler() == -1)
	{
		perror("signhandler");
	}

	while ( fgets(buffer, MAX_BUFFER, stdin) != NULL )
	{

		if (feof(stdin) )
			break;

		/* replace \n with a NULL terminator */
		buffer[strlen(buffer) - 1] = '\0';
		strncpy(data->message_buffer, buffer, sizeof(buffer));
		data->message_id = msgid++;
	}

	/* Set the exit value for message_id in the data segment */
	data->message_id = -1;

	if (shmdt(data) == -1 )
	{
		perror("shmdt");
		exit(4);
	}

	/* Wait to allow clients to read signal */
	sleep(0.1);
	shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

int set_signal_handler(void)
{
	int retval = 0;

	struct sigaction action;
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);

	if ( (retval = sigaction(SIGINT, &action, NULL)) != 0)
		return retval;

	return retval;
}

void signal_handler(int sig)
{
	switch(sig)
	{
		case SIGINT:
			msgid = -1;
			break;
		default:
			break;
	}
}
