#include "VoiceUDPRecive.h"
#include <iphlpapi.h>
#include <fstream>
#include "../imgui/ImguiControl.h"
#include "../3DObjectParticle/3DObjectParticle.h"

UDPStruct VoiceReciver::udp;
bool VoiceReciver::isShot;

bool VoiceReciver::isFront;
bool VoiceReciver::isBack;
bool VoiceReciver::isRight;
bool VoiceReciver::isLeft;
bool VoiceReciver::isPouse;
bool VoiceReciver::isWall;

void VoiceReciver::VoiceUDPUpdate()
{
	ReciveData();
}

void VoiceReciver::StartUp()
{
	udp.SetPortNum(50008);
	udp.StartUp();
}

void VoiceReciver::ReciveData()
{
	udp.Recive();
	std::string text = udp.GetResult();
	if (text == "") return;
	if (text == "SystemOpen")
	{
		Imgui::SetWindowActive(true);
	}
	else if (text == "SystemClose")
	{
		Imgui::SetWindowActive(false);
	}
	else if (text == "LinkStart")
	{

	}
	else if (text == "Explosion")
	{
		ObjectParticles::triangle.Init(XMFLOAT3(0, 0, 0), 10, ObjectParticle3D::ParticleType::Exprotion);
	}
	else if (text == "Shot")
	{
		SetIsShot(true);
	}
	else if (text == "Left")
	{
		SetLeft(true);
	}
	else if (text == "Right")
	{
		SetRight(true);
	}
	else if (text == "Front")
	{
		SetFront(true);
	}
	else if (text == "Back")
	{
		SetBack(true);
	}
	else if (text == "Pouse")
	{
		SetPouse(true);
	}
	else if (text == "Shield")
	{
		SetWall(true);
	}

}

void VoiceReciver::EndRecive()
{
	udp.EndRecive();
}
