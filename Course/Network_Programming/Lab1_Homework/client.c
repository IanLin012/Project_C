# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <unistd.h>

int main() {
	struct sockaddr_in server;
	int sock;
	printf("Enter Data: ");
	char buf[256];
	while(fgets(buf, sizeof(buf), stdin) != NULL) { //get all data of input line, and keep input until EOF
		for(int i=0; buf[i]!='\0'; i++) {
			if(buf[i]>='a' && buf[i]<='z') { //change small letter to uppercase
				buf[i]-=32;
			}
			if(buf[i]>='A' && buf[i]<='Z') { //change letter to next one
				buf[i]++;
				if(buf[i]>'Z') { //if letter is Z, change to A
					buf[i]='A';
				}
			}
		}
		bzero(&server, sizeof(server));
		server.sin_family = PF_INET;
		server.sin_addr.s_addr = inet_addr("127.0.0.1");
		server.sin_port = htons(5678);
		sock = socket(PF_INET, SOCK_STREAM, 0);
		connect(sock, (struct sockaddr*)&server, sizeof(server));
		send(sock, buf, sizeof(buf), 0); //send data to server
		printf("Send Data: %s", buf);
		int bytesReceived = recv(sock, buf, sizeof(buf)-1, 0); //receive data from server
		buf[bytesReceived] = '\0';
		printf("Received Response: %s\n", buf);
		printf("Enter Data: ");
	}
	close(sock);
}