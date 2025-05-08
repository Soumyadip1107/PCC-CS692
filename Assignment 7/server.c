#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFLEN 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client1, client2;
    char buffer[BUFLEN];
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int client1_connected = 0, client2_connected = 0;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for two clients...\n");

    while (1) {
        struct sockaddr_in sender_addr;
        socklen_t sender_len = sizeof(sender_addr);
        int n = recvfrom(sockfd, buffer, BUFLEN, 0, (struct sockaddr *)&sender_addr, &sender_len);
        buffer[n] = '\0';

        if (!client1_connected) {
            client1 = sender_addr;
            client1_connected = 1;
            printf("Client 1 connected. Waiting for Client 2...\n");
            const char *msg = "Waiting for second client...\n";
            sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&client1, sender_len);
        } 
        else if (!client2_connected) {
            if (memcmp(&sender_addr, &client1, sizeof(sender_addr)) == 0) {
                printf("Client 1 sent another message. Still waiting for Client 2...\n");
                const char *msg = "Still waiting for second client...\n";
                sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&client1, sender_len);
            } else {
                client2 = sender_addr;
                client2_connected = 1;
                printf("Client 2 connected. Chat started!\n");
                const char *msg = "Client connected. You can start chatting!\n";
                sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&client1, sizeof(client1));
                sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&client2, sizeof(client2));
            }
        } 
        else {
            if (memcmp(&sender_addr, &client1, sizeof(sender_addr)) == 0) {
                printf("Client 1: %s", buffer);
                sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client2, sizeof(client2));
            } 
            else if (memcmp(&sender_addr, &client2, sizeof(sender_addr)) == 0) {
                printf("Client 2: %s", buffer);
                sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client1, sizeof(client1));
            } 
            else {
                const char *reject_msg = "Chat is full. Only two clients allowed.\n";
                sendto(sockfd, reject_msg, strlen(reject_msg), 0, 
                      (struct sockaddr *)&sender_addr, sender_len);
                printf("Rejected connection from third client\n");
            }
        }
    }

    close(sockfd);
    return 0;
}

