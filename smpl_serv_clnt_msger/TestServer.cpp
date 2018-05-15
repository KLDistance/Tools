#include <unistd.h>
#include <iostream>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define RECV_LEN 1024
#define SEND_LEN 1024

#define DEFAULT_PORT 4096

class tcp_server
{
	private:
		int socket_fd, accept_fd;
		sockaddr_in myserver;
		sockaddr_in remote_addr;
	public: 
		tcp_server(int listen_port);
		int recv_msg();
};

tcp_server::tcp_server(int listen_port)
{
	if((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		throw("socket() failed");
	}
	
	memset(&myserver, 0, sizeof(myserver));
	myserver.sin_family = AF_INET;
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	myserver.sin_port = htons(listen_port);
	
	if(bind(socket_fd, (sockaddr*)&myserver, sizeof(myserver)) < 0)
	{
		throw("bind() failed");
	}
	
	if(listen(socket_fd, 10) < 0)
	{
		throw("listen() failed");
	}
}

int tcp_server::recv_msg()
{
	char confirmMsg[SEND_LEN] = "Dear user, this is Wilson's server!";
	size_t confirmMsg_Len = strlen(confirmMsg);
	while(1)
	{
		socklen_t sin_size = sizeof(sockaddr_in);
		if((accept_fd = accept(socket_fd, (sockaddr*)&remote_addr, &sin_size)) == -1)
		{
			throw("Accept error!");
			continue;
		}
		printf("Received a connection from %s\n", (char*)inet_ntoa(remote_addr.sin_addr));
		
		if(send(accept_fd, confirmMsg, SEND_LEN, 0) < 0)
		{
			throw("Send confirming message error!");
			continue;
		}
		
		if(!fork())
		{
			char buffer[RECV_LEN];
			memset(buffer, 0, RECV_LEN);
			if((read(accept_fd, buffer, RECV_LEN)) < 0)
			{
				throw("Read() error!");
			}
			else
			{
				printf("Received message: %s\n", buffer);
				break;
			}
			exit(0);
		}
		close(accept_fd);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		tcp_server ts(atoi(argv[1]));
		ts.recv_msg();
	}
	else
	{
		tcp_server ts(DEFAULT_PORT);
		ts.recv_msg();
	}
	return 0;
}
