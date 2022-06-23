#pragma once
#include <string>
using namespace std;

namespace ShlomonLibrary
{
	static int ConvertCharToInt(char str)
	{
		if (str == '0')
		{
			return 0;
		}
		else if (str == '1')
		{
			return 1;
		}
		else if (str == '2')
		{
			return 2;
		}
		else if (str == '3')
		{
			return 3;
		}
		else if (str == '4')
		{
			return 4;
		}
		else if (str == '5')
		{
			return 5;
		}
		else if (str == '6')
		{
			return 6;
		}
		else if (str == '7')
		{
			return 7;
		}
		else if (str == '8')
		{
			return 8;
		}
		else if (str == '9')
		{
			return 9;
		}
		else if (str == '10')
		{
			return 10;
		}
		else return -1;
	}
	static char ConvertIntToChar(int str)
	{
		if (str == 0)
		{
			return '0';
		}
		else if (str == 1)
		{
			return '1';
		}
		else if (str == 2)
		{
			return '2';
		}
		else if (str == 3)
		{
			return '3';
		}
		else if (str == 4)
		{
			return '4';
		}
		else if (str == 5)
		{
			return '5';
		}
		else if (str == 6)
		{
			return '6';
		}
		else if (str == 7)
		{
			return '7';
		}
		else if (str == 8)
		{
			return '8';
		}
		else if (str == 9)
		{
			return '9';
		}
		else return 'a';
	}
}