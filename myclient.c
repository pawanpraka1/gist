#include <netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE 1024
#define MSG "Hi Pawan Prakash Sharma"

int main()
{
	int sock, b, buf_len;
	char p[BUF_SIZE];
	struct sockaddr_in server_addr;
	struct hostent *host;

	signal(SIGPIPE, SIG_IGN);
	host = gethostbyname("localhost");
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(0);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7842);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	memset(&(server_addr.sin_zero),'\0',8);

	if((connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
	{
		perror("connect");
		exit(0);
	}
	sleep (1); // wait for server to be blocked on read
	while (1) {
		if(-1 == send(sock, MSG, sizeof(MSG), 0)) {
			perror("send");
			break;
		}
		printf("sent\n");
		break;
	}
	sleep(20);
	close(sock);
	return(0);
}
