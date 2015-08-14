#include "stdafx.h"
#include "HardwareManager.h"

Display * _display;
TouchInput * _touchInput;
Modem * _modem;

void HardwareManager::Init()
{
#ifdef SIMULATOR_BUILD
	_display = new Display_SDL();
#elif defined(PRODUCTION_BUILD)
    _display = new Display_ILI9341();
#else
    #error Unknown display
#endif

#ifdef SIMULATOR_BUILD
    _touchInput = new TouchInput_SDL();
#elif defined(PRODUCTION_BUILD)
    _touchInput = new TouchInput_FT6206();
#else
    #error Unknown touch input
#endif

#ifdef SIMULATOR_BUILD
    _modem = new Modem_Fake();
#elif defined(PRODUCTION_BUILD)
    _modem = new Modem_SIM800();
#else
    #error Unknown modem
#endif

    HardwareManager::GetDisplay()->Init();
    HardwareManager::GetTouchInput()->Init();

    //HardwareManager::GetModem()->Init(); // handled later in init process
}

Display * HardwareManager::GetDisplay()
{
    return _display;
}

TouchInput * HardwareManager::GetTouchInput()
{
    return _touchInput;
}

Modem * HardwareManager::GetModem()
{
    return _modem;
}

void HardwareManager::Cleanup()
{
    _display->Cleanup();
    _touchInput->Cleanup();

    _modem->Cleanup();

    delete _display;
    delete _touchInput;

    delete _modem;
}
