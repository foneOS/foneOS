#include "stdafx.h"
#include "App.h"


App::App(FoneOSString id)
{
	this->id = id;

	// TODO: error handling
	char * path = Utils::FoneOSStringToCharArray(Storage::GetFullPath(FoneOSString(STR("apps/")) + this->id + FoneOSString(STR("/desc.xml"))));
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path);
	this->name = Utils::CharArrayToFoneOSString((char *)doc.child("appDesc").child_value("name"));
}

App::~App()
{
}

int test(lua_State * _L)
{
	luaL_checkstring(_L, -1);

	const char * str = lua_tostring(_L, -1);
	Logging::LogMessage(Utils::CharArrayToFoneOSString((char*)str));
	lua_pop(_L, -1);

	return 0;
}

int Lua_layout_current(lua_State * _L)
{
	if (lua_gettop(_L) == 0)
	{
		// No arguments means return current
		luaW_push<FoneOSScreen>(_L, Layout::GetCurrentLayout());

		return 1;
	}
	FoneOSScreen* scr = luaW_check<FoneOSScreen>(_L, 1);
	bool addLast = true;
	if (lua_gettop(_L) >= 2)
	{
		addLast = *luaW_check<bool>(_L, 2);
	}

	Layout::SetCurrentLayout(scr, addLast);

	FoneOSPoint p = FoneOSPoint();
	p.x = 0;
	p.y = 0;
	scr->handleTouch(p);
	return 0;
}

int Lua_layout_draw(lua_State * _L)
{
	Layout::Draw();

	return 0;
}

int Lua_modem_call(lua_State * _L)
{
	const char * phoneNumber = luaL_checkstring(_L, 1);
	HardwareManager::GetModem()->Call(Utils::CharArrayToFoneOSString((char*)phoneNumber));
	return 0;
}

void App::Start()
{
	Logging::LogMessage(STR("Starting app..."));

	_L = luaL_newstate();

	luaL_openlibs(_L); // TODO: Remove dangerous libraries (io, system, etc)

	lua_newtable(_L); /* fone table */

		lua_newtable(_L); /* fone.layout */

			lua_pushcfunction(_L, &test);
			lua_setfield(_L, -2, "test");

			lua_pushcfunction(_L, &Lua_layout_current);
			lua_setfield(_L, -2, "current");

			lua_pushcfunction(_L, &Lua_layout_draw);
			lua_setfield(_L, -2, "draw");

		lua_setfield(_L, -2, "layout"); /* add layout table */

		lua_newtable(_L); /* fone.modem */

			lua_pushcfunction(_L, &Lua_modem_call);
			lua_setfield(_L, -2, "call");

		lua_setfield(_L, -2, "modem"); /* add modem table */

	lua_setglobal(_L, "fone"); /* globalize fone table */

	luaopen_Layout(_L);

	Logging::LogMessage(STR("Running app..."));
	std::string path = FoneOSString(STR("apps/")) + this->id + FoneOSString(STR("/lua/main.lua"));
	if (!Storage::FileExists(path))
	{
		Logging::LogMessage(STR("Error occurred in app!"));
		Logging::LogMessage(STR("App main.lua does not exist"));
		return;
	}

	int ret = luaL_dofile(_L,  (char*)Storage::GetFullPath(path).c_str());

	if (ret != 0)
	{
		Logging::LogMessage(STR("Error occurred in app!"));
		Logging::LogMessage(Utils::CharArrayToFoneOSString((char*)lua_tostring(_L, -1)));
	}
/*
	if (luaL_dostring(_L, "fone.layout.test('Hello from lua!')") == 1)
	{
		Logging::LogMessage(Utils::CharArrayToFoneOSString((char*)lua_tostring(_L, -1)));
	}*/
}

void App::SwitchTo()
{

}

void App::Stop()
{
	Logging::LogMessage(STR("Stopping app..."));
	lua_close(_L);
}
