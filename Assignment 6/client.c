#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

#define MAX 100

int main(){
	int port = 8080;

	int cid = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	caddr.sin_port = htons(port);
	
	printf("Client is Online....\n");
	while(1) {
		char dataword[MAX], codeword[MAX];
		
		printf("\nEnter the Dataword: ");
		fgets(dataword, MAX, stdin);
		dataword[strcspn(dataword, "\n")] = 0;
		
		sendto(cid, dataword, strlen(dataword) + 1, 0, (struct sockaddr*)&caddr, sizeof(caddr));
		
		if(strcmp(dataword, "end") == 0) {
			printf("Client terminated...\n");
			break;
		}
		socklen_t len = sizeof(caddr);
		int bytes_received = recvfrom(cid, codeword, MAX, 0, (struct sockaddr*)&caddr, &len);
		codeword[bytes_received] = '\0';
		printf("\nReceived Codeword: %s\n", codeword);
	}
	close(cid);
	return 0;
}
