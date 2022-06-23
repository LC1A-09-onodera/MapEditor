#pragma once
#include "../BaseDirectX/Input.h"

class InputKey
{
	
};

class Rewired
{
	list<KeyCode> keys;
	list<PadKeyCode> padKeys;
public:
	void KeyBoradInit(int count, ...);
	void PadInit(int count, ...);
	bool GetKey();
	bool GetKeyDown();
	bool GetKeyUp();
	void AddKey(KeyCode key);

	void LoadKey(const char *path);
};