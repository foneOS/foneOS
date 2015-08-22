#pragma once
class App
{
public:
	App(FoneOSString id);
	~App();

	// Starts the application.
	void Start();

	// Stops the application.
	void Stop();

	FoneOSString name;
	FoneOSString id;
	std::map<FoneOSString, FoneOSScreen> Layouts = {};
        std::stack<FoneOSScreen *> Stack;
private:
	lua_State * _L;
};
