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
	struct sockaddr_in r_addr;
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));

	bzero(&r_addr, sizeof(r_addr));
	r_addr.sin_family = AF_INET;
	r_addr.sin_port = htons(PORT_NO);
	r_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock_fd, (struct sockaddr*)&r_addr, sizeof(r_addr)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	char buff[MAX];
	while (true) {
		bzero(buff, sizeof(buff));
		recvfrom(sock_fd, buff, MAX, 0, NULL, NULL);
		printf("Broadcasted Message: %s\n", buff);
		if (strcmp(buff, "QUIT") == 0) break;
	}
	printf("RECEIVER: Broadcasting terminated by sender...\n");
	close(sock_fd);
	return 0;
}
