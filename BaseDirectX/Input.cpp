#include "Input.h"
#include "../Rewired/Rewired.h"
#include "../WindowsAPI/WinAPI.h"
#include "../imgui/ImguiControl.h"
#include <windows.h>
#include <windowsx.h>

IDirectInput8* Input::dinput = nullptr;
IDirectInputDevice8* Input::devkeyboard = nullptr;
DirectInput* Input::directInput = DirectInput::GetInstance();
float Input::mouseWheel = 0.0f;
void Input::Update()
{
	BaseDirectX::result = devkeyboard->Acquire();
	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
		keys[i] = 0;
	}
	BaseDirectX::result = devkeyboard->GetDeviceState(sizeof(keys), keys);

	directInput->UpdateInput();////////////////////////////////

	//マウス位置の取得
	//GetCursorPos(&mousePos);
	for (int i = 0; i < 10; i++)
	{
		oldMouseInput[i] = mouseInput[i];
		mouseInput[i] = false;
	}
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		mouseInput[VK_LBUTTON] = true;
	}
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		mouseInput[VK_RBUTTON] = true;
	}
	if (GetAsyncKeyState(VK_CANCEL))
	{
		mouseInput[VK_CANCEL] = true;
	}
	
	OnMouseWheel();
	
}

void Input::Init()
{
	//directInput.reset(DirectInput::GetInstance());
}

void Input::KeySet(WNDCLASSEX w, HWND hwnd)
{
	
	directInput->InputInit(w.hInstance,hwnd);//////////////////////////

	BaseDirectX::result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	BaseDirectX::result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	BaseDirectX::result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	//画面が手前にあるときのみキーボード入力を受け付ける
	//デバイスをこのアプリだけで占有しない
	//ウィルス等キーの無効化
	BaseDirectX::result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

bool Input::Key(BYTE key)
{
	return keys[key];
}

bool Input::Key(KeyCode key)
{
	return keys[(BYTE)key];
}

bool Input::KeyTrigger(BYTE key)
{
	return keys[key] && !oldkeys[key];
}

bool Input::KeyTrigger(KeyCode key)
{
	return keys[(BYTE)key] && !oldkeys[(BYTE)key];
}

bool Input::KeyEnd(BYTE key)
{
	return !keys[key] && oldkeys[key];
}

bool Input::KeyEnd(KeyCode key)
{
	return !keys[(BYTE)key] && oldkeys[(BYTE)key];
}

bool Input::MouseTrigger(MouseButton mouseinput)
{
	return mouseInput[(int)mouseinput] && !oldMouseInput[(int)mouseinput];
}

bool Input::Mouse(MouseButton mouseinput)
{
	return mouseInput[(int)mouseinput];
}

void Input::OnMouseWheel()
{
	mouseWheel = GET_WHEEL_DELTA_WPARAM(WindowsAPI::msg.wParam) / WHEEL_DELTA;
	Imgui::mouseWheel = mouseWheel;
	WindowsAPI::msg.wParam = 1;
}
