// foneOS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char **argv)
{
	Core::Init();
	while (true)
	{
		Core::Update();
	}
	return 0;
}

#ifdef WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(0, NULL);
}
#endif

#ifdef _MSC_VER
#undef main
int _tmain(int argc, FoneOSChar* argv[])
{
	#define main SDL_main
	return main(0, NULL);
}
#endif