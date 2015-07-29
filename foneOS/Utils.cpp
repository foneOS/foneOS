#include "stdafx.h"
#include "Utils.h"


Utils::Utils()
{
}

wchar_t * Utils::CharArrayToWChar(char * arr)
{
	size_t size = strlen(arr) + 1;
	size_t charsConverted = 0;
	wchar_t * ws = new wchar_t[size];
	mbstowcs(ws, arr, size);//mbstowcs_s(&charsConverted, ws, size, arr, _TRUNCATE);
	return ws;
}

char * Utils::WCharArrayToChar(const wchar_t * arr)
{
	size_t size = wcslen(arr) + 1;
	size_t charsConverted = 0;
	char * ws = new char[size];
	wcstombs(ws, arr, size); //wcstombs_s(&charsConverted, ws, size, arr, _TRUNCATE);
	return ws;
}

char* Utils::FoneOSStringToCharArray(FoneOSString str)
{
#ifdef UNICODE
	return Utils::WCharArrayToChar(str.c_str());
#else
	return strdup(str.c_str());
#endif
}

FoneOSString Utils::CharArrayToFoneOSString(char * arr)
{
#ifdef UNICODE
	return FoneOSString(Utils::CharArrayToWChar(arr));
#else
	return FoneOSString(arr);
#endif
}

FoneOSString Utils::IntToString(int t)
{
	FoneOSOStringStream os;
	os << t;
	return os.str();
}

void Utils::Delay(int millis)
{
    //SDL_Delay(millis);
    sleep(millis);
}

FoneOSString Utils::CondenseString(FoneOSString text)
{
	FoneOSIStringStream iss(text);
	FoneOSString retVal = STR("");
	FoneOSString s;
	while (iss >> s)
	{
		if (retVal != STR(""))
		{
			retVal += STR(" ") + s;
		}
		else
		{
			retVal = s;
		}
	}
	return retVal;
}

int Utils::NumberOfNewlines(FoneOSString text)
{
	int number = 0;
	for (unsigned int i = 0; i < text.length(); i++)
	{
		if (text[i] == '\n')
		{
			number++;
		}
	}
	return number;
}
