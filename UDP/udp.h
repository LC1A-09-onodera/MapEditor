#pragma once
#include <WinSock2.h> //windows
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

class UDPStruct
{
	WSAData wsaData;
	u_short port;
	int sock;
	//�A�h���X�̊i�[
	struct sockaddr_in addr;
	//�󂯎�������ʂ̊i�[
	std::string tex;
public:
	void SetPortNum(int port);
	void StartUp();
	void Recive();
	void EndRecive();
	std::string GetResult() {return tex;}
};