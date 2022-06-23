#pragma once
#include <Windows.h>

class WindowsAPI
{
public:
	static const int window_width = 1280;
	static const int window_height = 720;
	static HWND hwnd;
	static MSG msg;
	static WNDCLASSEX w;
	static bool Qite;
	static int intarval;
	static float rate;
	static void Set();
	static void Clear();
	static void CheckMsg();
	static void Update();
	static void Gethwnd();
	static POINT GetMousePos();
};