#include "stdafx.h"
#include "Modem.h"

static bool _enabled = false;

void Modem::Init()
{
	_enabled = true;
}

bool Modem::GetEnabled()
{
	return _enabled;
}

FoneOSString Modem::GetOperator()
{
	return STR("Fake Carrier");
}

int Modem::GetRSSI()
{
	return 20;
}

FoneOSString Modem::GetFWRev()
{
	return STR("FAKECO-1.2.3-456");
}

FoneOSString Modem::GetIMEI()
{
	return STR("123456789012345");
}

FoneOSString Modem::GetCCID()
{
	return STR("12345678901234567890");
}