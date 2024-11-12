#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> // for kill()

int main() {
    struct sockaddr_in server;
    int sock, readSize, client_id, msg_len = 0;
    char buf[] = "Hello", sendBuf[256], readBuf[256], message[256];
    pid_t pid;

    bzero(&server, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5678);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    connect(sock, (struct sockaddr*)&server, sizeof(server));
    client_id = 0;

    // 接收伺服器傳來的客戶端 ID
    read(sock, &client_id, sizeof(client_id));
    if (client_id < 1 || client_id > 2) {
        // 接收消息
        msg_len = read(sock, message, sizeof(message)-1);
        message[msg_len] = '\0';
        printf("%s\n", message);
        close(sock);
        return 0; // 跳出程式
    }

    printf("Connect order: %d\n", client_id);

    // 傳送buf內字串給Server
    write(sock, buf, sizeof(buf));
    printf("Send Message: %s\n\n", buf);

    pid = fork(); // 開始執行fork

    // Further code would go here (unavailable in the image)
    while(1) {
        // PID == -1 代表 fork 出錯
        if(pid == -1) {
            perror("Fork");
            break;
        }

        // PID == 0 代表是子程序
        else if(pid == 0) {
            read(sock, readBuf, sizeof(readBuf));
            printf("Read Message: %s\n\n", readBuf);
        }

        // PID > 0 代表是父程序
        else if(pid > 0) {
            // 判斷是否有EOF的發生
            if(scanf("%27[^\n]%*c", sendBuf) != EOF) {
                write(sock, sendBuf, sizeof(sendBuf));
                printf("Send Message: %s\n\n", sendBuf);
            }
            else {
                printf("EOF!!\n");
                // 傳送kill訊號結束pid下的process(解決無法成功釋放socket的問題)
                kill(pid, SIGKILL);
                break;
            }
        }
    }
    close(sock);
    return 0;
}
