#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <list>

#define User_Database "user_database.txt"
#define FileName_Database "fileName_database.txt"
#define PORT 1234

#define FAILIED 0
#define LOGIN 1
#define REGISTER 2
#define DOWNLOAD 3
#define UPLOAD 4
#define EXIT 5
#define GETLISTFILE 6
#define OK 1212
#define UPDATE 7
#define LOGOFF 8
#define STOPP 9

#define MAX_BYTE 1024 * 1024

#pragma comment (lib, "Ws2_32.lib")
#pragma once

class ServerCore
{
private:
	std::map<std::string, std::string> Users;
	std::vector<std::string> Files;

	int retcode;

	SOCKET socket_descriptor;
	sockaddr_in sin;

	bool stop = false;
	std::pair < std::string, std::string > messageLog;

	std::string msg;

	std::list<SOCKET> clients;

public:
	ServerCore();
	~ServerCore();

	void Start();

	void CreateDescriptor();
	void Bind();
	void Listen();
	bool Accept();
	void ReceiveData(SOCKET& client, char* bufferData);
	void ReceiveMessage(SOCKET& client, char* message);

	template <class T>
	void SendNumber(SOCKET& client, T number) {
		retcode = send(client, (char*)&number, sizeof(T), 0);
	}

	template <class T>
	void ReceiveNumber(SOCKET& client, T& number) {
		retcode = recv(client, (char*)&number, sizeof(T), 0);
	}

	void SendData(SOCKET& client, const char* bufferData, int size);

	void CloseSocket(SOCKET& socket, const char* username);

	//void errexit(const char* format, ...);

	void Handle(SOCKET& client /*, SOCKET& clientCommun */);

	void run();

	void SendListFile(SOCKET& client);
	void SendFile(SOCKET& client, std::string username);

	void ReceiveFile(SOCKET& client, std::string username);

	bool ReadFileUserDatabase();
	bool ReadFileListFileName();
	bool checkLogin(std::string username, std::string password);
	bool Register(std::string username, std::string password);

	std::vector<std::string> GetFiles();

	void Stop();

	std::pair<std::string, std::string> getMessage();

	void ReceiveRequest(SOCKET& client, int& request);

	void SetMessage(std::string msg);
};

