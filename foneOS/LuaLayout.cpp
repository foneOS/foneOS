#include "stdafx.h"

#include "LuaCustomTypes.hpp"
#include "LuaLayout.h"

int Lua_FoneOSScreen_Draw(lua_State * _L)
{
	FoneOSScreen* obj = luaW_check<FoneOSScreen>(_L, 1);
	obj->Draw();
	return 0;
}

static luaL_Reg FoneOSContainer_table[] =
{
	{ NULL, NULL }
};

static luaL_Reg FoneOSContainer_metatable[] =
{
	{ "x", luaU_getset<FoneOSContainer, int, &FoneOSContainer::x> },
	{ "y", luaU_getset<FoneOSContainer, int, &FoneOSContainer::y> },
	{ NULL, NULL }
};

static luaL_Reg FoneOSLabel_metatable[] =
{
	{ "text", luaU_getset<FoneOSLabel, FoneOSString, &FoneOSLabel::text> },
	{ NULL, NULL }
};

static luaL_Reg FoneOSScreen_metatable[] =
{
	{ "draw", Lua_FoneOSScreen_Draw },
	{ NULL, NULL }
};

int luaopen_Layout(lua_State* L)
{
	luaW_register<FoneOSContainer>(L,
		"FoneOSContainer",
		FoneOSContainer_table,
		FoneOSContainer_metatable//,
//		FoneOSContainer_new // If your class has a default constructor you can omit this argument,
				    // LuaWrapper will generate a default allocator for you.
	);

	luaW_register<FoneOSLabel>(L, "FoneOSLabel", FoneOSContainer_table, FoneOSLabel_metatable);
	luaW_extend<FoneOSLabel, FoneOSContainer>(L); // Informs the wrapper that FoneOSLabel inherits from the class FoneOSContainer

	luaW_register<FoneOSScreen>(L, "FoneOSScreen", FoneOSContainer_table, FoneOSScreen_metatable);

	return 1;
}
