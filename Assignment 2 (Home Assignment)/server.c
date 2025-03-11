#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAX 100

struct input_struct {
	char dataword[MAX];
	char divisor[MAX];
	char codeword[MAX];
	char remainder[MAX];
};

void XOR(char *dividend, const char *divisor) {
	for (int i = 0; divisor[i]; i++)
		dividend[i] = (dividend[i] == divisor[i]) ? '0' : '1';
}

void CRC(char *dataword, char *divisor, char *code, char *rem) {
	int dataLen = strlen(dataword), divLen = strlen(divisor);
	char dividend[MAX];
	
	strcpy(dividend, dataword);
	memset(dividend + dataLen, '0', divLen - 1);

	for (int i = 0; i <= dataLen; i++)
		if (dividend[i] == '1') XOR(dividend + i, divisor);

	
	strncpy(rem, &dividend[dataLen], divLen - 1);
	rem[divLen - 1] = '\0';
	
	strcpy(code, dataword);
	strcat(code, rem);
}

int main() {
	unlink("socket_server");

	struct sockaddr_un server_address, client_address;
	int server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "socket_server");

	bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
	listen(server_sockfd, 5);

	printf("Server running....\n\n");

	socklen_t client_len = sizeof(client_address);
	int client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
	
	while (1) {
		struct input_struct input, output;		
		
		read(client_sockfd, &input, sizeof(input));

		if (strcmp(input.dataword, "end") == 0) {
			printf("Terminating server.....\n");
			break;
		}
		printf("Received the dataword & divisor.\n");

		CRC(input.dataword, input.divisor, output.codeword, output.remainder);
		printf("Calculating......\n");
		write(client_sockfd, &output, sizeof(output));
		printf("Sent the codeword.....\n\n");
	}
	close(server_sockfd);

	return 0;
}

