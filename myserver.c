#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>   
#include <netinet/in.h>       
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define BUF_SIZE            1024
#define PORT_NUM            7842
#define PEND_CONNECTIONS     100
#define TRUE                   1
#define FALSE                  0


char *send_str = "pawan";
int lport = PORT_NUM;

void server_get_args(int argc, char * argv[])
{
	int c;
	while ((c=getopt(argc, argv, "s:p:")) != EOF)
	{
		switch(c)
		{
			case 's':
				send_str = optarg;
				break;
			case 'p':
				lport = atoi(optarg); 
				break;
			default:
				//usage();
				;
		}
	}
}


void *my_thread(void * arg)
{
	unsigned int    myClient_s;
	char           buf[BUF_SIZE];
	char           *temp;
	int            buf_len, len;

	myClient_s = *(unsigned int *)arg;
	while (1) {
		if (-1 == (len = recv(myClient_s, buf, 20, MSG_WAITALL))) {
			perror("recv");
			break;
		}
		write(1, buf, len);
	}
	close(myClient_s);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	unsigned int          server_s;
	struct sockaddr_in    server_addr;
	struct sockaddr_in    client_addr;
	int                   addr_len;
	pthread_attr_t        attr;
	pthread_t             threads;
	unsigned int	      client_s;

	server_s = socket(AF_INET, SOCK_STREAM, 0);

	int x = 1;
	if (setsockopt(server_s, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(x)) == -1) 
	{ 
		perror("Setsockopt");
		exit(FALSE);
	} 
	signal(SIGPIPE, SIG_IGN);
	
	server_get_args(argc, argv);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(lport);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (0 > bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
		perror("bind");
		exit(FALSE);
	}

	listen(server_s, PEND_CONNECTIONS);

	pthread_attr_init(&attr);
	while(TRUE)
	{
		addr_len = sizeof(client_addr);
		if (FALSE == (client_s = accept(server_s, (struct sockaddr *)&client_addr, &addr_len))) {
			perror("accept");
			continue;
		}

		int val = 80;
		if (0 > setsockopt(client_s, SOL_SOCKET, SO_RCVLOWAT, &val, sizeof (val))) {
			perror("low");
		}

		printf("a new client arrives ...\n");  
		{
			unsigned int ids;
			ids = client_s;
			if (pthread_create (&threads, &attr, my_thread, &ids))
				break;
		}
	}
	close (server_s);
	return (TRUE);
}
