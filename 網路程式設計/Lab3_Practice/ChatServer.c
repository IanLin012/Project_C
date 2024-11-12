#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> // for thread()
#include <stdlib.h> // for exit()

#define MAX_CLIENTS 2

void *connection_handler(void *);
int sockList[100] = {0}, client_id = 0;
pthread_mutex_t lock; // 用於保護 shared resources

int main() {
    struct sockaddr_in server, client;
    unsigned int sock, csock, addressSize;
    char buf[256], message[] = "Server is full!";
    pthread_t sniffer_thread;

    // 初始化mutex
    pthread_mutex_init(&lock, NULL);

    bzero(&server, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5678);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    bind(sock, (struct sockaddr*)&server, sizeof(server));
    listen(sock, 5);
    addressSize = sizeof(client);

    // 當有新的連線建立時
    while(1) {
        csock = accept(sock, (struct sockaddr*)&server, &addressSize);

        pthread_mutex_lock(&lock); // 加鎖，保護 shared resources

        client_id++; // 紀錄連線數+1
        send(csock, &client_id, sizeof(client_id), 0); // 傳送 client_id 給客戶端

        if(client_id > MAX_CLIENTS) {
            write(csock, message, strlen(message));
            close(csock); // 超過最大連線數，拒絕連線
            pthread_mutex_unlock(&lock); // 解鎖
            client_id--;
            continue;
        }

        printf("Client%d online!\n", client_id);

        // 記錄客戶端連線
        sockList[client_id - 1] = csock;

        pthread_mutex_unlock(&lock); // 解鎖

        // 建立thread，並且判斷是否建立成功，成功時回傳0
        if(pthread_create(&sniffer_thread, 0, connection_handler, (void *)&csock) != 0) {
            perror("Thread creation");
        }
        else {
            // 執行結束後釋放執行緒
            pthread_detach(sniffer_thread);
        }

        if(csock < 0) perror("Csock");
    }
    close(sock);
    printf("[Socket Close]\n");

    // 清理mutex
    pthread_mutex_destroy(&lock);

    return 0;
}

// 執行緒執行的功能
void *connection_handler(void* sock) {
    int csock = *(int *)sock;
    int readSize;
    char buf[256];

    while((readSize = read(csock, buf, sizeof(buf))) > 0) {
        pthread_mutex_lock(&lock); // 加鎖

        /// 廣播給所有已連線的client
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(sockList[i] != 0 && sockList[i] != csock) {
                write(sockList[i], buf, sizeof(buf));
            }
        }

        pthread_mutex_unlock(&lock); // 解鎖
        printf("Read Message: %s\n\n", buf);
    }

    // 當有client關閉連線時
    if(readSize == 0) {
        pthread_mutex_lock(&lock); // 加鎖
        printf("Client offline...\n");
        client_id--;

        // 取得斷線的csock存放位置
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(sockList[i] == csock)
                sockList[i] = 0;
        }

        pthread_mutex_unlock(&lock); // 解鎖
    }

    close(csock);
    printf("Client Close\n\n");
    pthread_exit(0); // 關閉thread
}
