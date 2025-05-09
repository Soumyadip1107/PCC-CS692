#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msgq.h"

int main() {
	MQ msg;
	int mqid = msgget(MQ_KEY, IPC_CREAT | 0666);

	msgrcv(mqid, (void *)&msg, DATASIZE,  3, 0);
	printf("Octal: %s\n",msg.mdata);
	return 0;
}
