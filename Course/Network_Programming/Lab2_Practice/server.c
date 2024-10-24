#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    struct sockaddr_in server, client;
    int sock, clientSock, addressSize, num1, num2, ans;
    fd_set rfds;
    struct timeval tv;
    int retval;

    bzero(&server, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5678);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    listen(sock, 1);
    
    addressSize = sizeof(client);
    clientSock = accept(sock, (struct sockaddr*)&client, &addressSize);

    while(1) {
        // Receive the first number
        recv(clientSock, &num1, sizeof(num1), 0);
        printf("Received first number: %d\n", num1);

        FD_ZERO(&rfds);
        FD_SET(clientSock, &rfds);

        tv.tv_sec = 3; // Wait for 3 seconds for the second number
        tv.tv_usec = 0;

        retval = select(clientSock + 1, &rfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select error!");
        } else if (retval) {
            // Second number is available
            recv(clientSock, &num2, sizeof(num2), 0);
            printf("Received second number: %d\n", num2);
            ans = num1 * num2;
        } else {
            // Timeout occurred, no second number
            printf("Timeout, no second number received.\n");
            ans = num1 * 100;
        }

        // Send the result back to the client
        send(clientSock, &ans, sizeof(ans), 0);
        printf("Sent result: %d\n", ans);
    }

    close(clientSock);
    close(sock);
    return 0;
}