/* File: server.cpp
 * Author: CRE
 * Last Edited: Mon Feb 27 15:59:22 2017
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int ServerSock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in ServerAddr;
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_port=htons(1234);
	//fprintf(stderr, "port: %d, ntohs:%d", ServerAddr.sin_port, ntohs(ServerAddr.sin_port));
	bind(ServerSock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));

	struct sockaddr_in ClientAddr;
	int ClientSock;
	while (1)
	{
	listen(ServerSock,20);

	socklen_t ClientAddrSize=sizeof(ClientAddr);
	int ClientSock=accept(ServerSock, (sockaddr*)&ClientAddr, &ClientAddrSize);
	fprintf(stderr, "received message from %s:%d", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));
	char str[]="Hello, world!";
	write(ClientSock,str, sizeof(str));
	close(ClientSock);
	}

	close(ServerSock);
	return 0;
}
