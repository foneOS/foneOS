#include "stdafx.h"
#include "HardwareManager.h"

Display * _display;

void HardwareManager::Init()
{
#ifdef SIMULATOR_BUILD
	_display = new Display_SDL();
#elif defined(PRODUCTION_BUILD)
    _display = new Display_ST7781R();
#else
    #error Unknown display
#endif
	HardwareManager::GetDisplay()->Init();
}

Display * HardwareManager::GetDisplay()
{
	return _display;
}

void HardwareManager::Cleanup()
{
	_display->Cleanup();

    delete _display;
}
