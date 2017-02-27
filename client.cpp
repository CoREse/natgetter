#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_SLEEP 100

int SleepCount=0;
int SleepInterval=10;
int LocalPort=2200;

int main(){
	//创建套接字
	int Sock = socket(AF_INET, SOCK_STREAM, 0);
	//向服务器（特定的IP和端口）发起请求
	struct sockaddr_in ServerAddr;
	memset(&ServerAddr, 0, sizeof(ServerAddr));  //每个字节都用0填充
	ServerAddr.sin_family = AF_INET;  //使用IPv4地址
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	ServerAddr.sin_port = htons(1234);  //端口

	sockaddr_in ClientAddr;
	ClientAddr.sin_family=AF_INET;
	//ClientAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ClientAddr.sin_port=htons(LocalPort);
	bind(Sock, (sockaddr*)&ClientAddr, sizeof(ClientAddr));
	struct sockaddr_in sin;
	socklen_t len=sizeof(sin);
	if (getsockname(Sock, (struct sockaddr *)&sin, &len)==-1)
	{
		perror("getsockname");
		return 1;
	}
	while (sin.sin_port!=ClientAddr.sin_port&&SleepCount<MAX_SLEEP)
	{
		fprintf(stderr, "bind port failed, try after %ds.\n", SleepInterval);
		sleep(SleepInterval);
		SleepCount+=SleepInterval;
		bind(Sock, (sockaddr*)&ClientAddr, sizeof(ClientAddr));
		if (getsockname(Sock, (struct sockaddr *)&sin, &len)==-1)
		{
			perror("getsockname");
			return 1;
		}
	}
	if (sin.sin_port!=ClientAddr.sin_port)
	{
		fprintf(stderr, "can't bind port! quit!\n");
		return 1;
	}

	connect(Sock, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));

	//读取服务器传回的数据
	char Buffer[40];
	read(Sock, Buffer, sizeof(Buffer)-1);

	printf("Message form server: %s\n", Buffer);

	//关闭套接字
	//shutdown(Sock,2);
	sleep(1);//to ensure server side close the connection not this side to avoid TIME_WAIT
	close(Sock);
	return 0;
}
