#pragma once

class HardwareManager
{
public:
	// Initializes the hardware manager and its associated hardware.
	static void Init();

	// Gets the current display.
	static Display * GetDisplay();
};