# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <unistd.h>

int main() {
	struct sockaddr_in server;
	int sock;
	char buf[256];
	printf("Enter Message: ");
	scanf("%s", buf);
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5678);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr*)&server, sizeof(server));
	send(sock, buf, sizeof(buf), 0);
	printf("Send Message: %s\n", buf);
	close(sock);
}