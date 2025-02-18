#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include "msgq.h"

void toBinary(int n, char *bin) {
	int i = (sizeof(int)*8);
	int bit;
	bin[i] = '\0';
	while(i > 0) {
		bit = n & 1;
		bin[--i] = bit + '0';
		n = n >> 1;
	}
}

void toOctal(int n, char *oct) {
	int i = (sizeof(int)*8)/3 + 1;
	int bit;
	oct[i] = '\0';
	while(i > 0) {
		bit = n & 7;
		oct[--i] = bit + '0';
		n = n >> 3;
	}
}

void toHexadecimal(int n, char *hex) {
	int i = (sizeof(int)*8)/4;
	int bit;
	hex[i] = '\0';
	while(i > 0) {
		bit = n & 15;
		if (bit < 10) hex[--i] = bit + '0';
		else hex[--i] = bit + 55;
		n = n >> 4;
	}
}

int main() {
	MQ msg;
	int mqid = msgget(MQ_KEY, IPC_CREAT | 0666);

	int n;
	printf("Enter a Number: ");
	scanf("%d",&n);

	char bin[(sizeof(int)*8) + 1], oct[(sizeof(int)*8)/3 + 2], hex[(sizeof(int)*8)/4 + 1];

	toBinary(n, bin);
	toOctal(n, oct);
	toHexadecimal(n, hex);

	// Send Binary
	msg.mtype = 2;
	strcpy(msg.mdata, bin);
	msgsnd(mqid, (void *)&msg, DATASIZE, 0);

	// Send Octal
	msg.mtype = 3;
	strcpy(msg.mdata, oct);
	msgsnd(mqid, (void *)&msg, DATASIZE, 0);

	// Send Hexadecimal
	msg.mtype = 4;
	strcpy(msg.mdata, hex);
	msgsnd(mqid, (void *)&msg, DATASIZE, 0);

	printf("Binary: %s Sent!\n",bin);
	printf("Octal: %s Sent!\n",oct);
	printf("Hexadecimal: %s Sent!\n",hex);

	sleep(10);
	msgctl(mqid, IPC_RMID, NULL);
	return 0;
}
