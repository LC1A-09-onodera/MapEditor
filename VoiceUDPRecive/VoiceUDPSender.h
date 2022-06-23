#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <WinSock2.h> //windows
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 

class VoiceUDPSender
{
	int port;
	int sock;
	struct sockaddr_in addr;
	WSAData wsaData;
	struct timeval tv;

	void StartUp();
	void SendData();
	void EndSend();
};
