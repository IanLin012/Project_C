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
	while(1) { //keep read data from client
		addressSize = sizeof(client);
		csock = accept(sock, (struct sockaddr*)&client, &addressSize);
		readSize = recv(csock, buf, sizeof(buf)-1, 0); // receive data from client
		buf[readSize] = '\0';
		printf("Read Data: %s", buf);
		//int let_cnt = 0,  num_cnt = 0;
		for(int i=0; buf[i]!='\0'; i++) {
			if(buf[i]>='A' && buf[i]<='Z') { //change letter to last one
				//let_cnt+=1;
				buf[i]--;
				if(buf[i]<'A') { //if letter is A, change to Z
					buf[i]='Z';
				}
			}
			else if(buf[i]>='0' && buf[i]<='9') {
				//num_cnt+=1;
			}
		}
		send(csock, buf, sizeof(buf), 0); // send data to client
		printf("Send Response: %s\n", buf);
	}
	close(sock);
}