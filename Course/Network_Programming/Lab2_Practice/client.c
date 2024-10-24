#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    fd_set rfds;
    struct sockaddr_in server;
    struct timeval tv;
    int sock, readSize, addressSize, retval, num1, num2, ans;

    bzero(&server, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5678);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    addressSize = sizeof(server);
    connect(sock, (struct sockaddr*)&server, sizeof(server));

    while(1) {
        printf("Enter first number: ");
        scanf("%d", &num1);

        // Send first number
        send(sock, &num1, sizeof(num1), 0);

        FD_ZERO(&rfds);
        FD_SET(0, &rfds); // Monitor standard input (stdin)

        tv.tv_sec = 3; // Wait for 3 seconds
        tv.tv_usec = 0;

        printf("Waiting for second number (3 seconds)...\n");
        retval = select(1, &rfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select error!");
        } else if (retval) {
            // Second number is available
            scanf("%d", &num2);
            send(sock, &num2, sizeof(num2), 0);
        } else {
            // Timeout occurred, no second number input, don't send anything more
            printf("Timeout, no second number entered.\n");
        }

        // Receive result from server
        readSize = recv(sock, &ans, sizeof(ans), 0);
        if (readSize > 0) {
            printf("Received result from server: %d\n", ans);
        }
    }

    close(sock);
    return 0;
}