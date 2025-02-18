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

	struct sockaddr_un server_addr,client_addr;
	
	unlink("socket_server");
	
	int server_sockfd=socket(AF_UNIX, SOCK_STREAM, 0);
	
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "socket_server");
	
	int server_len = sizeof(server_addr);
	
	bind(server_sockfd,(struct sockaddr *)&server_addr,server_len);
	
	listen(server_sockfd,5);
	
	printf("Server Running....\n");
	
	int client_len = sizeof(client_addr);
	int client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_addr,&client_len);
	
	char bit[MAX],count;
	while(1) {
		printf("\nWaiting....\n");
		sleep(2);
		
		read(client_sockfd,bit,MAX);
		if(strcmp(bit,"end")==0) {
			printf("Server Terminating....\n");
			break;
		}
		int i,count=0;
		for(i=0;i<strlen(bit);i++) {
			if(bit[i]=='1') count++;
		}
		printf("Count: %d\n\n",count);

		if(count%2 == 0) bit[i++] = '0';
		else bit[i++] = '1';
		
		bit[i] = '\0';
		
		write(client_sockfd,bit,strlen(bit)+1);
		printf("Sending back to the client\n");
	}
	close(server_sockfd);
	
	return 0;
}
	
	
