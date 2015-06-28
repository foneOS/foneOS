#pragma once
class Modem
{
public:
	// Initializes the modem.
	static void Init();

	// Checks if the modem is working.
	static bool GetEnabled();

	// Gets the operator the modem is connected to.
	static FoneOSString GetOperator();

	// Gets the RSSI of the modem's connection.
	static int GetRSSI();

	// Gets the firmware revision of the modem.
	static FoneOSString GetFWRev();

	// Gets the IMEI of the modem.
	static FoneOSString GetIMEI();

	// Gets the CCID of the modem SIM card.
	static FoneOSString GetCCID();
};

