#pragma once
class AppManager {
public:
	// Initializes the application manager.
	static void Init();

	// Opens the requested application.
	static void OpenApp(FoneOSString id);

	// Terminates the requested application.
	static void TerminateApp(FoneOSString id);

	// Frees memory of the application manager and its associated hardware.
	static void Cleanup();

	static std::map<FoneOSString, App *> Apps;
};