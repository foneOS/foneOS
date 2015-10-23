#include "stdafx.h"
#include "AppManager.h"

std::map<FoneOSString, App *> AppManager::Apps = std::map<FoneOSString, App *>();

void AppManager::Init()
{
}

void AppManager::OpenApp(FoneOSString id)
{
	if (AppManager::Apps.find(id) == AppManager::Apps.end())
	{
		// app is not open, start it
		App * app = new App(id);
		app->Start();
		AppManager::Apps[id] = app;
	}
}

void AppManager::TerminateApp(FoneOSString id)
{
}

void AppManager::Cleanup()
{
}
