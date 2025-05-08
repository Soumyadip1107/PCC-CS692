#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT_NO 8080
#define MAX 1024

int main() {
	struct sockaddr_in b_addr;
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));

	bzero(&b_addr, sizeof(b_addr));
	b_addr.sin_family = AF_INET;
	b_addr.sin_port = htons(PORT_NO);
	b_addr.sin_addr.s_addr = INADDR_BROADCAST;

	char buff[MAX];
	while (true) {
		bzero(buff, sizeof(buff));
		printf("Enter message (or type QUIT to exit): ");
		scanf(" %[^\n]", buff);

		sendto(sock_fd, buff, strlen(buff) + 1, 0,(struct sockaddr*)&b_addr, sizeof(b_addr));
		if (strcmp(buff, "QUIT") == 0) break;
	}
	printf("SENDER: Broadcast terminated...\n");
	close(sock_fd);
	return 0;
	
}
