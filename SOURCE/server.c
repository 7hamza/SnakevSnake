#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define err(msg) {fprintf(stderr, "%s\n", msg);exit(1);}
#define PORT 1234
#define ADDR INADDR_ANY
#define BACKLOG 10

static int clients[2];
static pthread_t threadpt[2];

static int server_socket()
{
	int n = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > n) err("socket()");

	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(ADDR);

	if(0 > bind(n, (struct sockaddr*)&server_addr, sizeof(server_addr))) err("bind()");
	if(0 > listen(n, BACKLOG)) err("listen()");

	return n;
}
static int server_accept(int p)
{
	struct sockaddr_in client_addr;
	unsigned len = sizeof(client_addr);

	int n = accept(p, (struct sockaddr*)&client_addr, &len);
	if(0 > n) err("accept()");
	return n;
}
static void* server_handler(void* arg)
{
	int n = *((int*)arg);
	int i = (n == clients[0])? 0 : 1;
	int j = (i+1) % 2;
	char dir = '\0';
	while(read(clients[i], &dir, sizeof(char*)))
	{
		write(clients[j], &dir, 1);
	}
	close(clients[i]);
	pthread_exit(0);
}
int main()
{
	short count = 0;
	int n = server_socket();
	while(2 != count)
	{
		clients[count] = server_accept(n);
		++count;
	}
	for(count = 0; count != 2; count++)
	{
		int pth = pthread_create(&threadpt[count], NULL, &server_handler, &clients[count]);
		if(0 > pth) err("pthread_create()");
	}
	char buf[2] = {'@','&'};
	for(count = 0; count != 2; count++)
	{
		write(clients[count], &buf[count], 1);
	}
	for(count = 0; count != 2; count++)
	{
		pthread_join(threadpt[count],NULL);
	}
	return 0;
}