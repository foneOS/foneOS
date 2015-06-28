#pragma once
class Core
{
public:
	Core();

	// Initializes the operating system.
	static void Init();

	// Runs the operating system.
	static void Update();

	// Clears the page stack.
	static void ClearStack();

	static std::stack<FoneOSScreen *> Stack;
};

