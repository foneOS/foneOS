#include "stdafx.h"
#include "HardwareManager.h"

Display * _display;
Modem * _modem;

void HardwareManager::Init()
{
#ifdef SIMULATOR_BUILD
	_display = new Display_SDL();
#elif defined(PRODUCTION_BUILD)
    _display = new Display_ST7781R();
#else
    #error Unknown display
#endif

#ifdef SIMULATOR_BUILD
    _modem = new Modem_Fake();
#elif defined(PRODUCTION_BUILD)
    _modem = new Modem_SIM800();
#else
    #error Unknown modem
#endif

    HardwareManager::GetDisplay()->Init();
    HardwareManager::GetModem()->Init();
}

Display * HardwareManager::GetDisplay()
{
    return _display;
}

Modem * HardwareManager::GetModem()
{
    return _modem;
}

void HardwareManager::Cleanup()
{
    _display->Cleanup();
    _modem->Cleanup();

    delete _display;
    delete _modem;
}
