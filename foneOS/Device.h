#pragma once
class Device
{
public:
	// Gets the device's serial number.
	static FoneOSString GetSerialNumber();

	// Gets the device's CPU's serial number.
	static FoneOSString GetCPUSerialNumber();

	// Gets the device's CPU's name.
	static FoneOSString GetCPUName();

	// Gets the device's CPU's architecture.
	static FoneOSString GetCPUArchitecture();
};

