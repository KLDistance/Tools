#include <Windows.h>
#include <winsock.h>
#include <iostream>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define RECV_LEN 1024
#define SEND_LEN 1024

#define IP_ADDR "98.142.142.155"
#define SERVER_PORT 4096

using namespace std;

int main()
{
	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iRet)
	{
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}
	
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		cout << "WSADATA version is not correct!" << endl;
		return -1;
	}

	//Create socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << endl;
		return -1;
	}

	//Initiation of server addr family
	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(SERVER_PORT);

	//Connect to the server
	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (iRet)
	{
		cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) exeute failed!" << endl;
		return -1;
	}
	
	//Receive message
	char recvBuf[RECV_LEN + 1];
	memset(recvBuf, 0, RECV_LEN + 1);
	recv(clientSocket, recvBuf, RECV_LEN, 0);
	cout << recvBuf << endl;

	//Sending message
	char sendBuf[SEND_LEN];
	sprintf_s(sendBuf, "Hello, this is client rabbit!");
	send(clientSocket, sendBuf, SEND_LEN + 1, 0);

	//Clear
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}