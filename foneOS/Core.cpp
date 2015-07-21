#include "stdafx.h"
#include "Core.h"


Core::Core()
{
}

void Core::Init()
{
	Logging::LogMessage(FoneOSString(STR("foneOS version ")) + FoneOSString(VERSION));
	Logging::LogMessage(FoneOSString(STR("File base path is ")) + Storage::GetFullPath(FoneOSString(STR(""))));

	bool ack = false;
	if (!Type::Init())
	{
		ack = true;
		Logging::LogMessage(STR("FREETYPE FAILURE"));
	}

	Logging::LogMessage(STR("Initializing hardware manager..."));
	HardwareManager::Init();

	// boot screen
	HardwareManager::GetDisplay()->Clear(COLOR_WHITE);

	if (ack)
	{
		HardwareManager::GetDisplay()->FillRectangle(20, 20, 50, 50, COLOR_RED);
		HardwareManager::GetDisplay()->FillRectangle(100, 100, 50, 50, COLOR_RED);
		HardwareManager::GetDisplay()->FillRectangle(20, 180, 50, 50, COLOR_RED);
		HardwareManager::GetDisplay()->FillRectangle(100, 260, 50, 50, COLOR_RED);
		HardwareManager::GetDisplay()->Flush();
		while (true) {}
	}

	HardwareManager::GetDisplay()->DrawImage(STR("boot.bmp"), 0, 0);
	HardwareManager::GetDisplay()->DrawImage(STR("logo.bmp"), 0, 0);
	HardwareManager::GetDisplay()->DrawString(VERSION, 0, 0, 1, COLOR_BLACK, COLOR_WHITE);

	HardwareManager::GetDisplay()->FillRectangle(0, 305, 240, 15, COLOR_WHITE);
	HardwareManager::GetDisplay()->DrawString(STR("Loading, please wait..."), 2, 307, 1, COLOR_BLACK, COLOR_WHITE);

	HardwareManager::GetDisplay()->Flush();

	Input::Init();
	Modem::Init();

	Layout::Init();

	Utils::Delay(2000); // TODO: Actually load.

	if (!Storage::FileExists(STR("dbsetup.txt")))
	{
		HardwareManager::GetDisplay()->FillRectangle(0, 305, 240, 12, COLOR_WHITE);
		HardwareManager::GetDisplay()->DrawString(STR("Setting up databases..."), 1, 307, 1, COLOR_BLACK, COLOR_WHITE);

		if (!Storage::FileExists(STR("callLog.sqlite3")))
		{
			if (
				!Database::ProvDB(STR("db/callLog.sqlite3"), STR("provscripts/db/callLog.sql"))
				
				)
			{
				HardwareManager::GetDisplay()->FillRectangle(0, 305, 240, 12, COLOR_WHITE);
				HardwareManager::GetDisplay()->DrawString(STR("DB setup error!"), 1, 307, 1, COLOR_BLACK, COLOR_WHITE);
				HardwareManager::GetDisplay()->Flush();
				while (true)
				{
					Utils::Delay(1);
				}
			}
		}
	}

	HardwareManager::GetDisplay()->Flush();

	Layout::Draw();
	App app = App();
	app.Start();
	app.Stop();
}

void Core::Update()
{
	HardwareManager::GetDisplay()->Update();

	Layout::Update();

	Utils::Delay(1);
}

void Core::ClearStack()
{
	while (!Core::Stack.empty())
	{
		Core::Stack.pop();
	}
}

std::stack<FoneOSScreen *> Core::Stack = std::stack<FoneOSScreen *>();