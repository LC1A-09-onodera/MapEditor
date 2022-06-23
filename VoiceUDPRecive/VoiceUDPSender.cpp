#include "VoiceUDPSender.h"

void VoiceUDPSender::StartUp()
{
	WSACleanup();
	WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)はwinsockのバージョン2.0ってこと
	sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50008);// 待ち受けポート番号を50008にする
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 送信アドレスを設定
}

void VoiceUDPSender::SendData()
{
	for (int i = 0; i < 100; i++) {
		/* AF_INET+SOCK_DGRAMなので、IPv4のUDPソケット */

		char data[] = "test";

		//データ送信
		sendto(sock, data, sizeof(data), 0, (struct sockaddr*)&addr, sizeof(addr));//addrに文字列送信
		printf("send data!\n");
		//Sleep(1000); //1000ms待つ（win32 API)
	}
}

void VoiceUDPSender::EndSend()
{
	closesocket(sock);
	WSACleanup();
}
