#pragma once
class Utils
{
public:
	Utils();

	// Converts a char array into a wchar array.
	static wchar_t * CharArrayToWChar(char * arr);

	// Converts a wchar array into a char array.
	static char * WCharArrayToChar(const wchar_t * arr);

	// Converts a FoneOSString into a char array.
	static char * FoneOSStringToCharArray(FoneOSString str);

	// Converts a char array into a FoneOSString.
	static FoneOSString CharArrayToFoneOSString(char * arr);

	// Converts an object into a string.
	static FoneOSString IntToString(int t);

	// Delays execution by the specified amount of milliseconds.
	static void Delay(int millis);

	// Condense multiple spaces into one.
	static FoneOSString CondenseString(FoneOSString text);

	// Counts the number of newlines in a string.
	static int Utils::NumberOfNewlines(FoneOSString text);
};

