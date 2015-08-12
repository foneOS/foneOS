#include "stdafx.h"
#include "Modem.h"

static bool _enabled = false;

void Modem_Fake::Init()
{
    _enabled = true;
}

bool Modem_Fake::GetEnabled()
{
    return _enabled;
}

FoneOSString Modem_Fake::GetOperator()
{
    return STR("Fake Carrier");
}

int Modem_Fake::GetRSSI()
{
    return 20;
}

FoneOSString Modem_Fake::GetFWRev()
{
    return STR("FAKECO-1.2.3-456");
}

FoneOSString Modem_Fake::GetIMEI()
{
    return STR("123456789012345");
}

FoneOSString Modem_Fake::GetCCID()
{
    return STR("12345678901234567890");
}

void Modem_Fake::Cleanup()
{
    _enabled = false;
}

static bool _senabled = false;

void Modem_SIM800::Init()
{
    _enabled = true;
}

bool Modem_SIM800::GetEnabled()
{
    return _enabled;
}

FoneOSString Modem_SIM800::GetOperator()
{
    return STR("Fake Carrier");
}

int Modem_SIM800::GetRSSI()
{
    return 20;
}

FoneOSString Modem_SIM800::GetFWRev()
{
    return STR("FAKECO-1.2.3-456");
}

FoneOSString Modem_SIM800::GetIMEI()
{
    return STR("123456789012345");
}

FoneOSString Modem_SIM800::GetCCID()
{
    return STR("12345678901234567890");
}

void Modem_SIM800::Cleanup()
{
    _senabled = false;
}
