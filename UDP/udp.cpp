#include "udp.h"

void UDPStruct::SetPortNum(int port)
{
	this->port = port;
}

void UDPStruct::StartUp()
{
	WSAStartup(MAKEWORD(2, 0), &wsaData);//MAKEWORD(2, 0)�̓o�[�W����
	sock = socket(AF_INET, SOCK_DGRAM, 0);//AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H

	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(port);   //�ʐM�|�[�g�ԍ��ݒ�
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANY�͂��ׂẴA�h���X����̃p�P�b�g����M����
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

void UDPStruct::Recive()
{
	char buf[100];
	memset(buf, 0, 100);
	int data_size = recv(sock, buf, 100, 0);
	tex = buf;
}

void UDPStruct::EndRecive()
{
	closesocket(sock);
	WSACleanup();
}