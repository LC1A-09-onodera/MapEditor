#include <WinSock2.h> //windows>
#include <iphlpapi.h>
#include "IpAddress.h"
#include "../imgui/ImguiControl.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

std::string IpAddress::ipAddress;

void IpAddress::GetIpAddress()
{
	//Imgui::GetIp(ipAddress.c_str());
}
