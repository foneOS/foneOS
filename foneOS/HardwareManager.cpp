#include "stdafx.h"
#include "HardwareManager.h"

Display * _display;

void HardwareManager::Init()
{
	_display = new Display_SDL();
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