#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>

#define MAX 100

int main() {
	struct sockaddr_un addr;
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "socket_server");
	
	printf("Client Running.....\n\n");
	
	int len = sizeof(addr);
	int result = connect(sockfd,(struct sockaddr *)&addr, len);
	
	if(result == -1) {
		printf("Cannot connect to server\n");
		exit(1);
	}
	
	char bit[MAX];
	
	while(1) {
		printf("Enter the bit-stream: ");
		scanf("%[^\n]",bit);
		getchar();
		
		printf("\nSending %s to the server\n",bit);
		write(sockfd,bit,strlen(bit)+1);
		
		if(strcmp(bit,"end")==0) {
			printf("Client Terminating....\n");
			break;
		}
		printf("Waiting for the server\n");
		sleep(1);
		read(sockfd,bit,MAX);
		printf("Received from the server with parity bit : %s\n\n",bit);
	}
	
	close(sockfd);
	return 0;
}
