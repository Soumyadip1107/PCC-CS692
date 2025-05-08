#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFLEN 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int sockfd;
pthread_mutex_t print_mutex;

void welcome() {
    printf("Welcome to ChatRoom\n");
}

void *receive_messages() {
    char buffer[BUFLEN];
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFLEN, 0, NULL, NULL);
        if (n > 0) {
            buffer[n] = '\0';

            pthread_mutex_lock(&print_mutex);
            printf("\rClient 2: %s", buffer);
            printf(  "You: "  );
            fflush(stdout);
            pthread_mutex_unlock(&print_mutex);
        }
    }
    return NULL;
}

int main() {
    welcome();
    struct sockaddr_in server_addr;
    char buffer[BUFLEN];
    pthread_t recv_thread;

    pthread_mutex_init(&print_mutex, NULL);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    printf("You: ");
    printf("Client joining chat\n");
    const char *init_msg = "Client joining chat\n";
    sendto(sockfd, init_msg, strlen(init_msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1) {
        pthread_mutex_lock(&print_mutex);
        printf("You: ");
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);

        fgets(buffer, BUFLEN, stdin);

        if (strcmp(buffer, "exit\n") == 0) {
            printf("Exiting chat...\n");
            break;
        }

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }

    pthread_mutex_destroy(&print_mutex);
    close(sockfd);
    return 0;
}

