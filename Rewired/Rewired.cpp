#include "Rewired.h"
#include <fstream>
#include <string>
#include <sstream>
void Rewired::KeyBoradInit(int count,...)
{
	va_list ap;
	va_start(ap, count);
	KeyCode key;
	for (int i = 0; i < count; i++)
	{
		key = va_arg(ap, KeyCode);
		if (key <= KeyCode::CodeNone) continue;
		keys.push_back(key);
	}
	va_end(ap);
}

void Rewired::PadInit(int count, ...)
{
	va_list ap;
	va_start(ap, count);
	PadKeyCode key;
	for (int i = 0; i < count; i++)
	{
		key = va_arg(ap, PadKeyCode);
		if (key <= PadKeyCode::ButtonNone) continue;
		padKeys.push_back(key);
	}
	va_end(ap);
}

bool Rewired::GetKey()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::Key(*keyItr))
		{
			return true;
		}
	}
	for (auto padItr = padKeys.begin(); padItr != padKeys.end(); ++padItr)
	{
		if (Input::directInput->IsButtonDown(*padItr))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::GetKeyDown()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::KeyTrigger(*keyItr))
		{
			return true;
		}
	}
	for (auto padItr = padKeys.begin(); padItr != padKeys.end(); ++padItr)
	{
		if (Input::directInput->IsButtonPush(*padItr))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::GetKeyUp()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::KeyEnd(*keyItr))
		{
			return true;
		}
	}
	for (auto padItr = padKeys.begin(); padItr != padKeys.end(); ++padItr)
	{
		if (Input::directInput->IsButtonUp(*padItr))
		{
			return true;
		}
	}
	return false;
}

void Rewired::AddKey(KeyCode key)
{
	keys.push_back(key);
}

void Rewired::LoadKey(const char* path)
{
	ifstream file;
	file.open(path);
	string keyType;
	while (getline(file, keyType))
	{
		istringstream line_stream(keyType);

		string key;
		getline(line_stream, key, ' ');
		if (key == "KeyBoard")
		{
			
		}
	}
}
