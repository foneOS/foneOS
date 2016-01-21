#include "stdafx.h"
#include "AppManager.h"

std::map<FoneOSString, App *> AppManager::Apps = std::map<FoneOSString, App *>();

void AppManager::Init()
{
}

void AppManager::OpenApp(FoneOSString id, bool shouldRestart)
{
	if (AppManager::Apps.find(id) == AppManager::Apps.end())
	{
		// app is not open, start it
		App * app = new App(id);
		app->Start();
		AppManager::Apps[id] = app;
	}
	else
	{
		if (shouldRestart)
		{
			AppManager::TerminateApp(id);
			AppManager::OpenApp(id, shouldRestart);
			return;
		}
		AppManager::Apps[id]->SwitchTo();
	}
}

void AppManager::TerminateApp(FoneOSString id)
{
	AppManager::Apps[id]->Stop();
	delete AppManager::Apps[id];
	AppManager::Apps.erase(id);
}

void AppManager::Cleanup()
{
}
