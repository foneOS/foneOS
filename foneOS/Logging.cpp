#include "stdafx.h"
#include "Logging.h"


Logging::Logging()
{
}


Logging::~Logging()
{
}


// Logs the provided message to the debug output.
void Logging::LogMessage(FoneOSString msg)
{
	FoneOSOut << msg.c_str() << FoneOSEndL;
	OutputDebugString(msg.c_str());
	OutputDebugString(FoneOSEndL);
}