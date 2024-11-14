#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

typedef struct Client {
    int sock;
    char room;
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char *message, char room, int exclude_sock);
void *handle_client(void *arg);

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5678);

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, MAX_CLIENTS);

    printf("Server is running...\n");

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);

        if (client_count >= MAX_CLIENTS) {
            char *msg = "Server is full!";
            send(client_sock, msg, strlen(msg), 0);
            close(client_sock);
            continue;
        }

        pthread_mutex_lock(&clients_mutex);

        clients[client_count].sock = client_sock;
        clients[client_count].room = 0;
        client_count++;

        pthread_mutex_unlock(&clients_mutex);

        pthread_t tid;
        pthread_create(&tid, NULL, &handle_client, (void *)&clients[client_count - 1]);
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}

void *handle_client(void *arg) {
    Client *cli = (Client *)arg;
    char buffer[BUFFER_SIZE];
    int room_selected = 0;

    // Client chooses a room
    while (!room_selected) {
        recv(cli->sock, buffer, sizeof(buffer), 0);
        if (buffer[0] == 'A' || buffer[0] == 'B' || buffer[0] == 'C') {
            cli->room = buffer[0];
            room_selected = 1;
            char join_msg[BUFFER_SIZE];
            snprintf(join_msg, sizeof(join_msg), "Client joined Room %c", cli->room);
            broadcast(join_msg, cli->room, cli->sock);
        } else {
            char *error_msg = "Invalid room. Choose A, B, or C.";
            send(cli->sock, error_msg, strlen(error_msg), 0);
        }
    }

    while (1) {
        int read_size = recv(cli->sock, buffer, sizeof(buffer), 0);
        if (read_size <= 0) {
            break;
        }

        buffer[read_size] = '\0';

        if (strcmp(buffer, "EXIT!") == 0) {
            break;
        } else if (strcmp(buffer, "list") == 0) {
            pthread_mutex_lock(&clients_mutex);
            int room_count = 0;
            for (int i = 0; i < client_count; i++) {
                if (clients[i].room == cli->room) {
                    room_count++;
                }
            }
            pthread_mutex_unlock(&clients_mutex);
            char list_msg[BUFFER_SIZE];
            snprintf(list_msg, sizeof(list_msg), "Clients in Room %c: %d", cli->room, room_count);
            send(cli->sock, list_msg, strlen(list_msg), 0);
            printf("Received 'list' request from a client in Room %c.\n", cli->room);
        } else {
            broadcast(buffer, cli->room, cli->sock);
        }
    }

    close(cli->sock);

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sock == cli->sock) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    char exit_msg[BUFFER_SIZE];
    snprintf(exit_msg, sizeof(exit_msg), "A client disconnected from Room %c.", cli->room);
    broadcast(exit_msg, cli->room, -1);

    return NULL;
}

void broadcast(char *message, char room, int exclude_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].room == room && clients[i].sock != exclude_sock) {
            send(clients[i].sock, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}