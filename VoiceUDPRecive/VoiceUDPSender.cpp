#include "VoiceUDPSender.h"

void VoiceUDPSender::StartUp()
{
	WSACleanup();
	WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)��winsock�̃o�[�W����2.0���Ă���
	sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50008);// �҂��󂯃|�[�g�ԍ���50008�ɂ���
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// ���M�A�h���X��ݒ�
}

void VoiceUDPSender::SendData()
{
	for (int i = 0; i < 100; i++) {
		/* AF_INET+SOCK_DGRAM�Ȃ̂ŁAIPv4��UDP�\�P�b�g */

		char data[] = "test";

		//�f�[�^���M
		sendto(sock, data, sizeof(data), 0, (struct sockaddr*)&addr, sizeof(addr));//addr�ɕ����񑗐M
		printf("send data!\n");
		//Sleep(1000); //1000ms�҂iwin32 API)
	}
}

void VoiceUDPSender::EndSend()
{
	closesocket(sock);
	WSACleanup();
}
