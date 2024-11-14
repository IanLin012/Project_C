#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 256

void *receive_messages(void *arg);

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(5678);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed.");
        return 1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, &receive_messages, &sock);

    printf("Enter room (A, B, or C): ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    send(sock, buffer, strlen(buffer), 0);

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "EXIT!") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            break;
        }

        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}

void *receive_messages(void *arg) {
    int sock = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        int read_size = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (read_size <= 0) {
            break;
        }
        buffer[read_size] = '\0';
        printf("%s\n", buffer);
    }

    return NULL;
}
