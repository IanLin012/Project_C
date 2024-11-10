# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <unistd.h>

int main() {
	fd_set rfds;
	struct sockaddr_in server;
	struct timeval tv;
	int sock, retval;
	char strA[100], strB[100], buffer[200];
	
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5678);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr*)&server, sizeof(server));

	while(1) {
		bzero(buffer, sizeof(buffer));
		// Input String A
		printf("String A: ");
		fgets(strA, 100, stdin);
		strA[strcspn(strA, "\n")] = '\0'; // Remove newline character
		if (feof(stdin)) break; // If EOF detected (CTRL+D), exit
		send(sock, strA, strlen(strA), 0);

		// Prepare for timeout and check String B
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		retval = select(1, &rfds, NULL, NULL, &tv);

		if(retval == -1) {
			perror("select error");
		}
		else if(retval) {
			// User input String B within 5 seconds
			fgets(strB, 100, stdin);
			strB[strcspn(strB, "\n")] = '\0';
			send(sock, strB, strlen(strB), 0);
		}
		else {
			// Timeout, String B is NULL
			strcpy(strB, "NULL");
			send(sock, strB, strlen(strB), 0);
		}

		// Receive and print the response from server
		recv(sock, buffer, sizeof(buffer), 0);
		printf("Server Response: %s\n", buffer);
	}

	close(sock);
	return 0;
}