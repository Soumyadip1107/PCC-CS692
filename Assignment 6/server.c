#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

#define MAX 100

void hamming(char *dataword, char *codeword) {
	int m = strlen(dataword);
	int r = 0;
	
	while (pow(2, r) < (m + r + 1)) r++;
	
	int sz = m + r;
	char temp[sz + 1];
	memset(temp, '0', sizeof(temp));
	temp[sz] = '\0';
	
	int j = 0;
	for(int i = 1; i <= sz; i++) 
		if((i & (i - 1)) != 0) 
			temp[i - 1] = dataword[j++];
			
	for(int i = 0; i < r; i++) {
		int pos = pow(2, i);
		int parity = 0;
		for(int k = pos - 1; k < sz; k += 2 * pos)
			for(int j = k; j < k + pos && j < sz; j++)
				if(temp[j] == '1')
					parity ^= 1;
					
		temp[pos - 1] = (parity == 1) ? '1' : '0';
	}
	strcpy(codeword, temp);
}

int main() {
	int port = 8080;
	int sid = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(port);
	
	bind(sid, (struct sockaddr*)&saddr, sizeof(saddr));
	
	printf("Server is Online.....\n");
	
	while(1) {
		char dataword[MAX], codeword[MAX];
		
		socklen_t len = sizeof(caddr);
		int bytes_received = recvfrom(sid, dataword, MAX, 0, (struct sockaddr*)&caddr, &len);
		dataword[bytes_received] = '\0';
		
		if(strcmp(dataword, "end") == 0) {
			printf("Server terminated...\n");
			break;
		}
		printf("\nReceived Dataword: %s\n", dataword);		
		hamming(dataword, codeword);		
		printf("\nGenerated Codeword: %s\n", codeword);
		
		sendto(sid, codeword, strlen(codeword) + 1, 0, (struct sockaddr*)&caddr, sizeof(caddr));
	}
	close(sid);
	return 0;
}
