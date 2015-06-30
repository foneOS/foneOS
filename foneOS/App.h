#pragma once
class App
{
public:
	App();
	~App();

	// Starts the application.
	void Start();

	// Stops the application.
	void Stop();

	FoneOSString Name;
	std::map<FoneOSString, FoneOSScreen> Layouts = {};
private:
	lua_State * _L;
};

