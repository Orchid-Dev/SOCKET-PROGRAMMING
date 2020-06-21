#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <WS2tcpip.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#define PORT 1234

#define MAX_BYTE 1024 * 1024

#define CLLOGIN 1
#define CLREGISTER 2
#define CLDOWNLOAD 3
#define CLUPLOAD 4
#define CLEXIT 5
#define CLGETLISTFILE 6
#define OK 1212
#define FAILIED 0
#define UPDATE 7
#define LOGOFF 8 
#define STOP 9

#pragma comment (lib, "Ws2_32.lib")
#pragma once

class ClientCore
{
private:
	WORD wVersionRequested;
	WSADATA wsaData;
	int retcode;

	SOCKET socket_descriptor;

	char ServerName[64];
	HOSTENT* pHostEnt;
	sockaddr_in sin;

	std::vector<std::string> Files;

	std::string username;

public:
	ClientCore();
	~ClientCore();

	void CreateDescriptor();

	bool GetServer(std::string hostAddr);
	bool Connect();

	template <class T>
	void SendNumber(T number) {
		retcode = send(socket_descriptor, (char*)&number, sizeof(T), 0);
		if (retcode == SOCKET_ERROR)
			errexit("Send number failed: %d\n", WSAGetLastError());
	}

	template <class T>
	void ReceiveNumber(T& number) {
		retcode = recv(socket_descriptor, (char*)&number, sizeof(T), 0);
		if (retcode == SOCKET_ERROR)
			errexit("Receive number failed: %d\n", WSAGetLastError());
	}

	void ReceiveData(char* bufferData);
	void ReceiveMessage(char* message);

	void SendData(const char* bufferData, int size);

	void Close();

	int Login(std::string username, std::string password);
	int Register(std::string username, std::string password);

    void ReceiveListFile();
	bool GetFile(int noFile);
	bool createFile(int noFile, std::string filePath);

	void UploadFile(std::string filePath);

	void Disconnect(int flag);

	void errexit(const char* format, ...);

	std::vector<std::string> GetFiles();
};
