#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX 100

char r[MAX];
int ac = 0;

int count(int n) {
	int c = 0;
	while (n) {
		c++;
		n /= 10;
	}
	return c;
}

void check(char *input) {
	int len = strlen(input);
	int i, c = 0, s = 0, cp = 0;

	if (strcmp(input, "0.0.0.0") == 1) {
		strcpy(r, "YES");
		return;
	}
	strcpy(r, "YES");

	if (len < 7 || len > 15) {
		strcpy(r, "NO");
		return;
	}
	strcat(input, ".");

	for (i = 0; i <= len; i++) {
		if (input[i] == '.') {
			cp++;
			if (c == 0 || s > 255 || cp > 4) {
				strcpy(r, "NO");
				break;
			}
			if (c > 1 && input[i - c] == '0') {
				strcpy(r, "NO");
				break;
			}
			c = 0;
			s = 0;
		}
		if (input[i] >= '0' && input[i] <= '9') {
		c++;
		s = (s * 10) + (input[i] - '0');
		}
	}
	if (cp != 4) { strcpy(r, "NO"); }
}


void *client(void *arg) {
	int cfd = *((int *)arg);
	struct sockaddr_in caddr;
	char input[MAX], result[MAX];
	socklen_t client_len = sizeof(caddr);

	getpeername(cfd, (struct sockaddr *)&caddr, &client_len);
	printf("Client %ld : %s : %d Connected\n", pthread_self(), inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

	ac++;

	while (1) {
	printf("Waiting..\n");
	read(cfd, input, MAX);
	
	printf("Server Received : %s\n", input);
	if (strcmp(input, "end") == 0) {
		printf("Client %ld : %s : %d Disconnected\n", pthread_self(), inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
		close(cfd);
		break;
	}
	check(input);
	strcpy(result, r);

	printf("Sending result to client %ld : %s : %d\n", pthread_self(), inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
		write(cfd, result, strlen(result) + 1);
	}
	ac--;
	if (ac == 0) {
		printf("No more clients\n");
		printf("Server Ending..\n");
		exit(0);
	}
	pthread_exit(NULL);
	}

	int main() {
	pthread_t t;
	int sfd, cfd;
	struct sockaddr_in saddr, caddr;
	socklen_t client_len = sizeof(caddr);

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(8888);

	bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
	listen(sfd, 5);

	printf("Server Running\n");

	while (1) {
	cfd = accept(sfd, (struct sockaddr *)&caddr, &client_len);
	printf("Client connected from %s:%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

		if (pthread_create(&t, NULL, client, (void *)&cfd) != 0) {
			perror("pthread_create failed");
			close(cfd);
		} else { pthread_detach(t);}
	}
	close(sfd);
	return 0;
}

