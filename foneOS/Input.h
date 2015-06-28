#pragma once
class Input
{
public:
	// Initializes the input device.
	static void Init();

	// Sends a touch event at the point.
	static void SendTouch(FoneOSPoint point);

	// Gets a touch event.
	static FoneOSPoint GetTouch();
};

