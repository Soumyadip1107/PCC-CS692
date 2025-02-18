#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "msgq.h"

void receive_sorted_data(int mqid) {
	Message msg;

	msgrcv(mqid, &msg, sizeof(msg) - sizeof(long), 12, 0);
	printf("\nSorted by Name:\n");
	for (int i = 0; i < msg.count; i++)
		printf("%s	%d\n", msg.students[i].name, msg.students[i].roll);

	msgrcv(mqid, &msg, sizeof(msg) - sizeof(long), 13, 0);
	printf("\nSorted by Roll Number:\n");
	for (int i = 0; i < msg.count; i++)
		printf("%s	%d\n", msg.students[i].name, msg.students[i].roll);
}

int main() {
	Message msg;
	int mqid = msgget(MQ_KEY, IPC_CREAT|0666);
	
	int n;
	printf("Enter number of students: ");
	scanf("%d", &n);
	printf("\n");

	Student students[n];
	for (int i = 0; i < n; i++) {
		printf("Enter name and roll number: ");
		scanf("%s %d", students[i].name, &students[i].roll);
	}
	
	// Send to receiver_name.c
	msg.mtype = 21;
	msg.count = n;
	memcpy(msg.students, students, n * sizeof(Student));
	msgsnd(mqid, &msg, sizeof(msg) - sizeof(long), 0);
	
	printf("\nmsg sent to receiver_name successfully.\n");
	
	// Send to receiver_roll.c
	msg.mtype = 31;
	msg.count = n;
	memcpy(msg.students, students, n * sizeof(Student));
	msgsnd(mqid, &msg, sizeof(msg) - sizeof(long), 0);	

	printf("msg sent to receiver_roll successfully.\n");
	
	receive_sorted_data(mqid);

	msgctl(mqid, IPC_RMID, NULL);

	return 0;
}

