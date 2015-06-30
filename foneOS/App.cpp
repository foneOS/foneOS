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


void App::Start()
{
	Logging::LogMessage(STR("Starting app..."));

	_L = luaL_newstate();

	luaL_openlibs(_L); // TODO: Remove dangerous libraries (io, system, etc)

	lua_newtable(_L); /* fone table */

		lua_newtable(_L); /* fone.layout */

			lua_pushcfunction(_L, &test);
			lua_setfield(_L, -2, "test");

		lua_setfield(_L, -2, "layout"); /* add layout table */

	lua_setglobal(_L, "fone"); /* globalize fone table */

	if (luaL_dostring(_L, "fone.layout.test('Hello from lua!')") == 1)
	{
		Logging::LogMessage(Utils::CharArrayToFoneOSString((char*)lua_tostring(_L, -1)));
	}
}


void App::Stop()
{
	Logging::LogMessage(STR("Stopping app..."));
	lua_close(_L);
}