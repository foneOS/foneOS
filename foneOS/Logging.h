#pragma once
class Logging
{
public:
	Logging();
	~Logging();

	// Logs the provided message to the debug output.
	static void LogMessage(FoneOSString msg);
};

