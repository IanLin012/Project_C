# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <unistd.h>

int main() {
	struct sockaddr_in server, client;
	int sock, clientSock, addressSize;
	char strA[100], strB[100], buffer[200];
	
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5678);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	bind(sock, (struct sockaddr*)&server, sizeof(server));
	listen(sock, 5);

	addressSize = sizeof(client);
	clientSock = accept(sock, (struct sockaddr*)&client, &addressSize);

	while(1) {
		bzero(buffer, sizeof(buffer));
		// Receive String A
		bzero(strA, sizeof(strA));
		recv(clientSock, strA, sizeof(strA), 0);
		if (strlen(strA) == 0) break;  // If empty message, EOF received
		printf("String A: %s\n", strA);

		// Receive String B
		bzero(strB, sizeof(strB));
		recv(clientSock, strB, sizeof(strB), 0);
		printf("String B: %s\n", strB);
		
		if(strcmp(strB, "NULL") == 0) {
			// If String B is NULL, calculate parity and length of String A
			int lengthA = strlen(strA);
			if(lengthA % 2 == 0) {
				sprintf(buffer, "even, string A = %d", lengthA);
			}
			else {
				sprintf(buffer, "odd, string A = %d", lengthA);
			}
		}
		else {
			// If String B is provided, concatenate and send back
			sprintf(buffer, "%s,%s", strB, strA);
		}

		// Send response to client
		send(clientSock, buffer, strlen(buffer), 0);
	}

	close(clientSock);
	close(sock);
	return 0;
}