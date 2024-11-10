# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <unistd.h>

int main() {
	struct sockaddr_in server, client;
	int sock, csock, readSize, addressSize;
	char buf[256];
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5678);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	bind(sock, (struct sockaddr*)&server, sizeof(server));
	listen(sock, 5);
	while(1) {
		addressSize = sizeof(client);
		csock = accept(sock, (struct sockaddr*)&client, &addressSize);
		readSize = recv(csock, buf, sizeof(buf)-1, 0);
		/*if(readSize <= 0) {
			close(csock);
			break;
		}*/
		buf[readSize] = '\0';
		printf("Read Message: %s\n", buf);
	}
	close(sock);
}