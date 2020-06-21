#include "pch.h"
#include "ClientCore.h"
#include <string>
#include <iostream>
#include <conio.h>

#pragma warning(disable: 4996)

using namespace std;

ClientCore::ClientCore() {
	wVersionRequested = MAKEWORD(2, 2);	// Use MAKEWORD(1,1) if you're at WinSock 1.1
	retcode = WSAStartup(wVersionRequested, &wsaData);
	
	if (LOBYTE(wsaData.wVersion) != LOBYTE(wVersionRequested) ||
		HIBYTE(wsaData.wVersion) != HIBYTE(wVersionRequested))
	{
		WSACleanup();
	}
}

ClientCore::~ClientCore() {
	retcode = WSACleanup();
}

void ClientCore::CreateDescriptor() {
	socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
}

bool ClientCore::GetServer(string hostAddr) {
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	in_addr ip = { 0 };

	ip.s_addr = inet_addr(hostAddr.c_str());

	if (pHostEnt = gethostbyaddr((char*)&ip, 4, AF_INET)) {
		memcpy(&sin.sin_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);
	}
	else {
		return false;
	}

	return true;
}

bool ClientCore::Connect() {
	retcode = connect(socket_descriptor, (sockaddr*)&sin, sizeof(sin));
	if (retcode == SOCKET_ERROR) {
		return false;
	}

	return true;
}

void ClientCore::ReceiveData(char* bufferData) {
	int sizeData;

	ReceiveNumber(sizeData);
	recv(socket_descriptor, bufferData, sizeData, 0);
}

void ClientCore::ReceiveMessage(char* message) {
	int msgLen;

	ReceiveNumber(msgLen);
	recv(socket_descriptor, message, msgLen, 0);

	message[msgLen] = 0;
}

void ClientCore::SendData(const char* bufferData, int size) {
	SendNumber(size);

	retcode = send(socket_descriptor, bufferData, size, 0);
}

void ClientCore::Close() {
	retcode = closesocket(socket_descriptor);
}

int ClientCore::Login(string username, string password) {
	SendData(username.c_str(), username.length());
	SendData(password.c_str(), password.length());

	char msg[1000];

	ReceiveMessage(msg);

	if (string(msg).compare("login successfully") == 0)
		return 0;

	if (string(msg).compare("Server disconnect") == 0)
		return 1;

	return 2;
}

int ClientCore::Register(string username, string password) {
	SendData(username.c_str(), username.length());
	SendData(password.c_str(), username.length());

	char msg[1000];

	ReceiveMessage(msg);

	if (string(msg).compare("register successfully") == 0)
		return 0;

	if (string(msg).compare("Server disconnect") == 0)
		return 1;

	return 2;
}

void ClientCore::ReceiveListFile() {
	Files.clear();
	int n_Files;
	ReceiveNumber(n_Files);

	for (int i = 0; i < n_Files; ++i) {
		char file[1000];

		ReceiveMessage(file);

		Files.push_back(file);
		cout << i + 1 << ". " << file << endl;
	}
}

bool ClientCore::GetFile(int noFile) {
	SendNumber(noFile);

	char msg[100];
	ReceiveMessage(msg);

	if (string(msg).compare("OK") != 0) {
		return false;
	}
	
	return true;
}

bool ClientCore::createFile(int noFile, string filePath) {
	unsigned long long sizeFile;
	
	FILE* file = fopen(filePath.c_str(), "wb");

	ReceiveNumber(sizeFile);

	char* bufferData = new char[MAX_BYTE];

	while (sizeFile >= MAX_BYTE) {
		ReceiveData(bufferData);
		fwrite(bufferData, MAX_BYTE, 1, file);

		sizeFile -= MAX_BYTE;
	}

	if (sizeFile > 0) {
		ReceiveData(bufferData);
		fwrite(bufferData, (int)sizeFile, 1, file);
	}

	fclose(file);

	delete[] bufferData;

	return true;
}

void ClientCore::UploadFile(string filePath) {
	FILE* file = fopen(filePath.c_str(), "rb");

	char name[100];
	int i = filePath.length() - 1;
	int j = 0;

	while (i >= 0 && filePath[i] != '\\') {
		name[j++] = filePath[i--];
	}

	name[j] = '\0';

	strrev(name);

	SendData(name, strlen(name));

	fseek(file, 0, SEEK_END);
	unsigned long long sizeFile = ftell(file);
	fseek(file, 0, SEEK_SET);

	SendNumber(sizeFile);

	char* bufferData = new char[MAX_BYTE];

	while (sizeFile >= MAX_BYTE) {
		fread(bufferData, MAX_BYTE, 1, file);
		SendData(bufferData, MAX_BYTE);

		sizeFile -= MAX_BYTE;
	}

	if (sizeFile > 0) {
		fread(bufferData, (int)sizeFile, 1, file);
		SendData(bufferData, (int)sizeFile);
	}

	fclose(file);

	delete[] bufferData;
}

void ClientCore::Disconnect(int flag) {
	if (flag == 1) 
		SendNumber(CLEXIT);

	if (flag == 3)
		SendNumber(LOGOFF);

	Close();
}

void ClientCore::errexit(const char* format, ...)
{
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	WSACleanup();
	exit(1);
}

vector<string> ClientCore::GetFiles() {
	return Files;
}