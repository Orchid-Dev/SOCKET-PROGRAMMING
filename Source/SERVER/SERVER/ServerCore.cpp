#include "pch.h"
#include <vector>
#include <thread>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#pragma warning(disable: 4996)

using namespace std;

ServerCore::ServerCore() {

}

void ServerCore::Start() {
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	retcode = WSAStartup(wVersionRequested, &wsaData);
	if (retcode != 0)
		//errexit("Startup failed: %d\n", retcode);

		if (LOBYTE(wsaData.wVersion) != LOBYTE(wVersionRequested) ||
			HIBYTE(wsaData.wVersion) != HIBYTE(wVersionRequested)) {
			printf("Supported version is too low\n");
			WSACleanup();
		}

	ReadFileUserDatabase();
	ReadFileListFileName();
	clients.clear();
	stop = false;
}

ServerCore::~ServerCore() {
	retcode = WSACleanup();
	if (retcode == SOCKET_ERROR) {}
		//errexit("Cleanup failed: %d\n", WSAGetLastError());

	printf("Return Code: %d\n", retcode);
}

void ServerCore::CreateDescriptor() {
	socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == INVALID_SOCKET) {}

}

void ServerCore::Bind() {
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = INADDR_ANY;

	retcode = bind(socket_descriptor, (sockaddr*)&sin, sizeof(sin));
	if (retcode == SOCKET_ERROR) {}
		//errexit("Bind failed: %d\n", WSAGetLastError());
}

void ServerCore::Listen() {
	retcode = listen(socket_descriptor, 0);
	if (retcode == SOCKET_ERROR) {}
		//errexit("Listen failed: %d\n", WSAGetLastError());
}

bool ServerCore::Accept() {
	sockaddr_in new_sin;

	SOCKET client;

	int addrlen;

	addrlen = sizeof(new_sin);
	client = accept(socket_descriptor, (sockaddr*)&new_sin, &addrlen);
	if (client == INVALID_SOCKET) {
	///	errexit("Accept failed: %d\n", WSAGetLastError());
		client = 0;
		return false;
	}

	char ClientAddr[4];
	HOSTENT* pClientHostEnt = NULL;

	memcpy(ClientAddr, &new_sin.sin_addr, sizeof(ClientAddr));
	pClientHostEnt = gethostbyaddr(ClientAddr, sizeof(ClientAddr), PF_INET);

	if (pClientHostEnt == NULL) {
	//	errexit("Get host by address failed: %d\n", GetLastError());
		return false;
	}
	
	if (stop) {
		return false;
	}
		
	clients.push_back(client);
	return true;
}

void ServerCore::ReceiveData(SOCKET& client, char* bufferData) {
	int sizeData;

	ReceiveNumber(client, sizeData);
	recv(client, bufferData, sizeData, 0);
}

void ServerCore::ReceiveMessage(SOCKET& client, char* message) {
	int msgLen;

	ReceiveNumber(client, msgLen);

	recv(client, message, msgLen, 0);

	if (msgLen != SOCKET_ERROR) {
		message[msgLen] = 0;
	}
}

void ServerCore::SendData(SOCKET& client, const char* bufferData, int size) {
	SendNumber(client, size);

	retcode = send(client, bufferData, size, 0);
	if (retcode == SOCKET_ERROR) {}
}

void ServerCore::CloseSocket(SOCKET& socket, const char* username) {
	retcode = closesocket(socket);

	if (username != NULL) {
		messageLog.first = username;
		messageLog.second = string(" disconnect");

		msg = messageLog.first + messageLog.second;
	}
	else {
		messageLog.first = "";
		messageLog.second = "";
	}
}

void ServerCore::Handle(SOCKET& client) {
	int request = 0;
	bool stop = false;

	while (!stop) {
		//SendNumber(client, OK);
		ReceiveRequest(client, request);;

		if (request == LOGIN) {
			char username[1000];
			char password[1000];

			ReceiveMessage(client, username);
			ReceiveMessage(client, password);

			if (this->stop) {
				SendData(client, "Server disconnect", string("Server disconnect").length());
				stop = true;
			}
			else if (!checkLogin(username, password))
				SendData(client, "login fail", string("login fail").length());
			else {
				SendData(client, "login successfully", string("login successfully").length());

				messageLog.first = username;
				messageLog.second = string(" log in");

				msg = messageLog.first + messageLog.second;

				while (1) {
					ReceiveRequest(client, request);

					if (request == DOWNLOAD) {
						SendFile(client, username);
					}
					else if (request == UPLOAD) {
						ReceiveFile(client, username);
					}
					else if (request == EXIT) {
						CloseSocket(client, username);
						
						stop = true;
						break;
					}
					else if (request == GETLISTFILE) {
						SendListFile(client);
						SendData(client, msg.c_str(), msg.size());
					}
					else if (request == LOGOFF) {
						CloseSocket(client, NULL);
						
						messageLog.first = username;
						messageLog.second = string(" log off");

						msg = messageLog.first + messageLog.second;

						stop = true;
						break;
					}
				}
			}
		}
		else if (request == REGISTER) {
			char username[1000];
			char password[1000];

			ReceiveMessage(client, username);
			ReceiveMessage(client, password);

			if (this->stop) {
				SendData(client, "Server disconnect", string("Server disconnect").length());
				stop = true;
			}
			else if (Register(username, password)) {
				SendData(client, "register successfully", string("register successfully").length());

				messageLog.first = username;
				messageLog.second = string(" register + log in");

				string msg = messageLog.first + messageLog.second;

				while (1) {
					ReceiveRequest(client, request);

					if (request == DOWNLOAD) {
						SendFile(client, username);
					}
					else if (request == UPLOAD) {
						ReceiveFile(client, username);
					}
					else if (request == EXIT) {
						CloseSocket(client, username);
						stop = true;
						break;
					}
					else if (request == GETLISTFILE) {
						SendListFile(client);
						SendData(client, msg.c_str(), msg.size());
					}
					else if (request == LOGOFF) {
						CloseSocket(client, NULL);

						messageLog.first = username;
						messageLog.second = string(" log off");

						msg = messageLog.first + messageLog.second;

						stop = true;
						break;
					}
						
				}
			}
			else {
				SendData(client, "register fail", string("register fail").length());
			}
		}

		else if (request == EXIT) {
			CloseSocket(client, NULL);
			stop = true;
		}
	}
}

void ServerCore::run() {
	Listen();

	int index = 0;

	while (!stop) {
		if (Accept()) {
			thread t(&ServerCore::Handle, this, std::ref(clients.back()));
	
			t.detach();
		}
	}

	//clients.clear();
	closesocket(socket_descriptor);
	retcode = WSACleanup();
}

bool ServerCore::ReadFileUserDatabase() {
	ifstream user_database(User_Database);

	if (user_database.fail()) {
		cout << "Can not read User_Database" << endl;
		return false;
	}

	Users.clear();

	string username;
	string password;

	while (!user_database.eof()) {
		user_database >> username;
		user_database >> password;

		Users[username] = password;
	}

	user_database.close();
}

bool ServerCore::ReadFileListFileName() {
	Files.clear();

	ifstream fileName_database(FileName_Database);

	if (fileName_database.fail()) {
		cout << "Can not read List_FileName" << endl;
		return false;
	}

	char fileName[1000];
	while (!fileName_database.eof()) {
		fileName_database.getline(fileName, 1000, '\n');
		Files.push_back(fileName);
	}

	Files.pop_back();

	fileName_database.close();
}

bool ServerCore::checkLogin(string username, string password) {
	map<string, string>::iterator it;

	it = Users.find(username);

	if (it == Users.end())
		return false;

	if (it->second.compare(password) != 0)
		return false;

	return true;
}

bool ServerCore::Register(string username, string password) {
	if (Users.find(username) != Users.end()) {
		return false;
	}

	Users[username] = password;

	fstream user_database(User_Database, ios::app);

	user_database << username << " " << password << endl;
	user_database.close();

	return true;
}

void ServerCore::SendListFile(SOCKET& client) {
	int n_Files = Files.size();

	SendNumber(client, n_Files);

	for (auto file : Files) {
		SendData(client, file.c_str(), file.length());
	}
}

void ServerCore::SendFile(SOCKET& client, std::string username) {
	int noFile;
	ReceiveNumber(client, noFile);

	if (noFile <= 0 || noFile > Files.size()) {
		SendData(client, "NOT OK", string("NOT OK").length());
		return;
	}

	SendData(client, "OK", string("OK").length());

	messageLog.first = username;
	messageLog.second = string(" download file ") + Files[noFile - 1];

	FILE* file = fopen(string(".\\data\\" + Files[noFile - 1]).c_str(), "rb");

	if (file == NULL) {
		cout << "Can not open " << Files[noFile - 1] << endl;
		return;
	}

	fseek(file, 0, SEEK_END);
	unsigned long long sizeFile = ftell(file);
	fseek(file, 0, SEEK_SET);

	SendNumber(client, sizeFile);

	char* bufferData = new char[MAX_BYTE];

	while (sizeFile >= MAX_BYTE) {
		fread(bufferData, MAX_BYTE, 1, file);
		SendData(client, bufferData, MAX_BYTE);

		sizeFile -= MAX_BYTE;
	}

	if (sizeFile > 0) {
		fread(bufferData, (int)sizeFile, 1, file);
		SendData(client, bufferData, (int)sizeFile);
	}

	fclose(file);

	delete[] bufferData;
}

void ServerCore::ReceiveFile(SOCKET& client, std::string username) {
	char fileName[1000];
	ReceiveMessage(client, fileName);

	bool flag = 1;
	int count = 0;
	string fileTemp;
	string chs = "_(";
	
	while (flag) {
		fileTemp = string(fileName);

		if (count > 0)
			if (fileTemp.find('.') != string::npos)
				fileTemp.insert(fileTemp.find('.'), string(chs + std::to_string(count) + ')'));
			else
				fileTemp += string(chs + std::to_string(count) + ')');

		int index;
		bool flag2 = 1;
		for (index = 0; index < Files.size() && flag2; ++index) {
			if (Files[index].compare(fileTemp) == 0) {
				flag2 = 0;
				++count;
			}
		}

		if (index == Files.size() && flag2)
			flag = 0;
	}

	ofstream fileName_database(FileName_Database, ios::app);
	fileName_database << fileTemp << endl;
	fileName_database.close();

	ReadFileListFileName();

	messageLog.first = username; 
	messageLog.second = string(" upload file ") + fileName + string(" ==> ") + fileTemp;

	FILE* file = fopen(string(".\\data\\" + fileTemp).c_str(), "wb");

	unsigned long long sizeFile;
	ReceiveNumber(client, sizeFile);

	char* bufferData = new char[MAX_BYTE];

	while (sizeFile >= MAX_BYTE) {
		ReceiveData(client, bufferData);
		fwrite(bufferData, MAX_BYTE, 1, file);

		sizeFile -= MAX_BYTE;
	}

	if (sizeFile > 0) {
		ReceiveData(client, bufferData);
		fwrite(bufferData, (int)sizeFile, 1, file);
	}

	fclose(file);

	delete[] bufferData;
}

//void ServerCore::errexit(const char* format, ...)
//{
//	va_list	args;
//
//	va_start(args, format);
//	vfprintf(stderr, format, args);
//	va_end(args);
//	WSACleanup();
// 	exit(1);
//}

vector<string> ServerCore::GetFiles() {
	return Files;
}

void ServerCore::Stop() {
	stop = true;

	SOCKET socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	in_addr ip = { 0 };

	ip.s_addr = inet_addr("127.0.0.1");

	HOSTENT* pHostEnt;

	if (pHostEnt = gethostbyaddr((char*)&ip, 4, AF_INET)) {
		memcpy(&sin.sin_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);
	}

	connect(socket_descriptor, (struct sockaddr*) & sin, sizeof(sin));

	closesocket(socket_descriptor);	
}

pair<string, string> ServerCore::getMessage() {
	pair<string, string> res = messageLog;

	messageLog.first.clear();
	messageLog.second.clear();

	return res;
}

void ServerCore::ReceiveRequest(SOCKET& client, int& request) {
	ReceiveNumber(client, request);
}

void ServerCore::SetMessage(string msg) {
	this->msg = msg;
}