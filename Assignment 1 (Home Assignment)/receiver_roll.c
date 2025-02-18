#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "msgq.h"

int compare_roll(const void *a, const void *b) {
	return ((Student *)a)->roll - ((Student *)b)->roll;
}

int main() {
	int mqid = msgget(MQ_KEY, IPC_CREAT|0666);
	Message msg;

	msgrcv(mqid, (void *)&msg, sizeof(msg) - sizeof(long), 31, 0);
	printf("Before Sort: \n");
	for (int i = 0; i < msg.count; i++)
		printf("%s	%d\n", msg.students[i].name, msg.students[i].roll);

	qsort(msg.students, msg.count, sizeof(Student), compare_roll);
	
	printf("\nAfter Sort: \n");
	for (int i = 0; i < msg.count; i++)
		printf("%s	%d\n", msg.students[i].name, msg.students[i].roll);

	msg.mtype=13;
	msgsnd(mqid, (void *)&msg, sizeof(msg) - sizeof(long), 0);
	printf("\nmsg sent to sender.\n");

	return 0;
}
