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

int main() {
	struct sockaddr_un address;
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "socket_server");
	
	printf("Client running....\n\n");

	connect(sockfd, (struct sockaddr *)&address, sizeof(address));	

	while (1) {
		struct input_struct input, output;

		printf("Enter Binary Data Word: ");
		scanf("%s", input.dataword);

		if (strcmp(input.dataword, "end") == 0) {
			write(sockfd, &input, sizeof(input)); 
			printf("\nTerminating client.....\n");
			break;
		}

		printf("Enter Binary Divisor: ");
		scanf("%s", input.divisor);

		write(sockfd, &input, sizeof(input));
		read(sockfd, &output, sizeof(output));

		printf("Codeword: %s\n", output.codeword);
		printf("Remainder: %s\n\n", output.remainder);
	}

	close(sockfd);
	return 0;
}

