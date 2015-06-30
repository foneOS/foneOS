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

	Logging::LogMessage(STR("Initializing display..."));
	Display::Init();

	// boot screen
	Display::Clear(COLOR_WHITE);

	if (ack)
	{
		Display::FillRectangle(20, 20, 50, 50, COLOR_RED);
		Display::FillRectangle(100, 100, 50, 50, COLOR_RED);
		Display::FillRectangle(20, 180, 50, 50, COLOR_RED);
		Display::FillRectangle(100, 260, 50, 50, COLOR_RED);
		Display::Flush();
		while (true) {}
	}

	Display::DrawImage(STR("boot.bmp"), 0, 0);
	Display::DrawImage(STR("logo.bmp"), 0, 0);
	Display::DrawString(VERSION, 0, 0, 1, COLOR_BLACK, COLOR_WHITE);

	Display::FillRectangle(0, 305, 240, 15, COLOR_WHITE);
	Display::DrawString(STR("Loading, please wait..."), 2, 307, 1, COLOR_BLACK, COLOR_WHITE);

	Display::Flush();

	Input::Init();
	Modem::Init();

	Layout::Init();

	Utils::Delay(2000); // TODO: Actually load.

	if (!Storage::FileExists(STR("dbsetup.txt")))
	{
		Display::FillRectangle(0, 305, 240, 12, COLOR_WHITE);
		Display::DrawString(STR("Setting up databases..."), 1, 307, 1, COLOR_BLACK, COLOR_WHITE);

		if (!Storage::FileExists(STR("callLog.sqlite3")))
		{
			if (
				!Database::ProvDB(STR("db/callLog.sqlite3"), STR("provscripts/db/callLog.sql"))
				
				)
			{
				Display::FillRectangle(0, 305, 240, 12, COLOR_WHITE);
				Display::DrawString(STR("DB setup error!"), 1, 307, 1, COLOR_BLACK, COLOR_WHITE);
				Display::Flush();
				while (true)
				{
					Utils::Delay(1);
				}
			}
		}
	}

	Display::Flush();

	Layout::Draw();
	App app = App();
	app.Start();
	app.Stop();
}

void Core::Update()
{
	Display::Update();

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