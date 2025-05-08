#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 100

int main(int argc, char **argv) {
	int sockfd, len = sizeof(struct sockaddr_in);
	struct sockaddr_in address = {AF_INET, htons(8888), inet_addr("127.0.0.1")};
	char input[MAX], res[MAX];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	connect(sockfd, (struct sockaddr *)&address, len);
	while (1) {
		printf("Enter the ip address (a.b.c.d): ");
		scanf("%[^\n]", input); getchar();
		write(sockfd, input, strlen(input) + 1);
		if (strcmp(input, "end") == 0) break;
		read(sockfd, res, MAX);
		printf("IP Address %s is %s\n", input, (strcmp(res, "YES") == 0) ? "VALID" : "NOT VALID");
	}
	close(sockfd);
	return 0;
}

