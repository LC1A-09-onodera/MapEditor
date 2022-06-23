#pragma once
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <windef.h>
//#include <Windows.h>
#include <string>
#include "BaseDirectX.h"
#include "DirectInput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

static BYTE keys[256] = {};
static BYTE oldkeys[256] = {};
static LPDIRECTINPUT8 g_lpDI;
static LPDIRECTINPUTDEVICE8 g_lpDIDevive;
static DIDEVCAPS g_diDevCaps;
static POINT mousePos{};
static bool mouseInput[10] = { false, false, false, false , false , false , false , false , false , false };
static bool oldMouseInput[10] = { false,false, false , false , false , false , false , false , false , false };

enum class KeyCode
{
	CodeNone = -1,
	A = DIK_A,
	B = DIK_B,
	C = DIK_C,
	D = DIK_D,
	E = DIK_E,
	F = DIK_F,
	G = DIK_G,
	H = DIK_H,
	I = DIK_I,
	J = DIK_J,
	K = DIK_K,
	L = DIK_L,
	M = DIK_M,
	N = DIK_N,
	O = DIK_O,
	P = DIK_P,
	Q = DIK_Q,
	R = DIK_R,
	S = DIK_S,
	T = DIK_T,
	U = DIK_U,
	V = DIK_V,
	W = DIK_W,
	X = DIK_X,
	Y = DIK_Y,
	Z = DIK_Z,
	Key1 = DIK_1,
	Key2 = DIK_2,
	Key3 = DIK_3,
	Key4 = DIK_4,
	Key5 = DIK_5,
	Key6 = DIK_6,
	Key7 = DIK_7,
	Key8 = DIK_8,
	Key9 = DIK_9,
	Key0 = DIK_0,
	Tab = DIK_TAB,
	Space = DIK_SPACE,
	LShift = DIK_LSHIFT,
	RShift = DIK_RSHIFT,
	UpArrow = DIK_UPARROW,
	DownArrow = DIK_DOWNARROW,
	RightArrow = DIK_RIGHTARROW,
	LeftArrow = DIK_LEFTARROW,
	KeyCodeMax = 45,

};
static const char KeyCodeString[] = "A\0B\0C\0D\0E\0F\0G\0H\0I\0J\0K\0L\0M\0N\0O\0P\0Q\0R\0S\0T\0U\0V\0W\0X\0Y\0Z\0Key1\0Key2\0Key3\0Key4\0Key5\0Key6\0Key7\0Key8\0Key9\0Key0\0Tab\0Space\0LShift\0RShift\0Up\0Down\0Right\0Left\0\0";

enum class MouseButton
{
	NONE,
	LBUTTON,
	RBUTTON,
};
static const char MouseCode[] = "LButton\0RButton\0\0";

class Input
{
public:
	static IDirectInput8 *dinput;
	static IDirectInputDevice8 *devkeyboard;
	static DirectInput *directInput;/////////////////////
	static float mouseWheel;
	static void Init();
	static void Update();

	static void KeySet(WNDCLASSEX w, HWND hwnd);

	static bool Key(BYTE key);
	static bool Key(KeyCode key);

	static bool KeyTrigger(BYTE key);
	static bool KeyTrigger(KeyCode key);

	static bool KeyEnd(BYTE key);
	static bool KeyEnd(KeyCode key);

	static bool MouseTrigger(MouseButton mouseinput);

	static bool Mouse(MouseButton mouseinput);

	static void OnMouseWheel();

};