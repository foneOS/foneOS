#include "stdafx.h"
#include "App.h"


App::App()
{
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

	return 0;
}

int Lua_layout_draw(lua_State * _L)
{
	Layout::Draw();

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

	lua_setglobal(_L, "fone"); /* globalize fone table */

	luaopen_Layout(_L);

	FoneOSString path = Storage::GetFullPath(STR("apps/foneOS.demoApp/lua/main.lua"));
	int ret = luaL_dofile(_L,  (char*)path.c_str());

	if (ret != 0)
	{
		Logging::LogMessage(STR("Error occured in app!"));
		Logging::LogMessage(Utils::CharArrayToFoneOSString((char*)lua_tostring(_L, -1)));
	}
/*
	if (luaL_dostring(_L, "fone.layout.test('Hello from lua!')") == 1)
	{
		Logging::LogMessage(Utils::CharArrayToFoneOSString((char*)lua_tostring(_L, -1)));
	}*/
}


void App::Stop()
{
	Logging::LogMessage(STR("Stopping app..."));
	lua_close(_L);
}
