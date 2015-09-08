#include "stdafx.h"

#include "LuaCustomTypes.hpp"
#include "LuaLayout.h"

// TODO: maybe template these add functions?
int Lua_FoneOSScreen_AddLabel(lua_State * _L)
{
	FoneOSScreen* obj = luaW_check<FoneOSScreen>(_L, 1);
	FoneOSLabel* lbl = luaW_check<FoneOSLabel>(_L, 2);
	obj->labels.push_back(*lbl);
	return 0;
}

int Lua_FoneOSScreen_AddTitle(lua_State * _L)
{
        FoneOSScreen* obj = luaW_check<FoneOSScreen>(_L, 1);
        FoneOSTitle* title = luaW_check<FoneOSTitle>(_L, 2);
        obj->titles.push_back(*title);
        return 0;
}

int Lua_FoneOSScreen_AddButton(lua_State * _L)
{
	FoneOSScreen* obj = luaW_check<FoneOSScreen>(_L, 1);
	FoneOSButton* btn = luaW_check<FoneOSButton>(_L, 2);
	obj->buttons.push_back(*btn);
	std::cout << "Printing btn" << std::endl;
	std::cout << obj->buttons[0].text<< std::endl;
	return 0;
}

int Lua_FoneOSScreen_Draw(lua_State * _L)
{
	FoneOSScreen* obj = luaW_check<FoneOSScreen>(_L, 1);
	obj->Draw();
	return 0;
}

int Lua_FoneOSContainer_Create(lua_State * _L)
{
	FoneOSContainer* obj = luaW_check<FoneOSContainer>(_L, 1);
	obj->Create();
	return 0;
}

void Lua_FoneOSButton_callbackHandler(FoneOSContainer* cont)
{
	FoneOSButton* btn = (FoneOSButton*)cont;
	lua_State * _L = (lua_State*)btn->metadata["state"];
	lua_rawgeti(_L, LUA_REGISTRYINDEX, *(int*)btn->metadata["onActivate"]); // retrieve the function
	lua_pcall(_L, 0, 0, 0);
}

int Lua_FoneOSButton_onActivate(lua_State * _L)
{
	// TODO: Parameter checking
	FoneOSButton* btn = luaW_check<FoneOSButton>(_L, 1);
	int funcId = luaL_ref(_L, LUA_REGISTRYINDEX);
	btn->metadata["onActivate"] = (void*)new int(funcId);
	btn->metadata["state"] = (void*)_L;
	btn->onActivate = Lua_FoneOSButton_callbackHandler;
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
	{ "create", Lua_FoneOSContainer_Create },
	{ NULL, NULL }
};

static luaL_Reg FoneOSLabel_metatable[] =
{
	{ "text", luaU_getset<FoneOSLabel, FoneOSString, &FoneOSLabel::text> },
	{ NULL, NULL }
};

static luaL_Reg FoneOSTitle_metatable[] =
{
	{ "text", luaU_getset<FoneOSTitle, FoneOSString, &FoneOSTitle::text> },
	{ NULL, NULL }
};

static luaL_Reg FoneOSButton_metatable[] =
{
	{ "text", luaU_getset<FoneOSButton, FoneOSString, &FoneOSButton::text> },
	{ "onActivate", Lua_FoneOSButton_onActivate },
	{ NULL, NULL }
};

static luaL_Reg FoneOSScreen_metatable[] =
{
	{ "addLabel", Lua_FoneOSScreen_AddLabel },
	{ "addTitle", Lua_FoneOSScreen_AddTitle },
	{ "addButton", Lua_FoneOSScreen_AddButton },

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

	luaW_register<FoneOSTitle>(L, "FoneOSTitle", FoneOSContainer_table, FoneOSTitle_metatable);
	luaW_extend<FoneOSTitle, FoneOSContainer>(L); // Informs the wrapper that FoneOSTitle inherits from the class FoneOSContainer

	luaW_register<FoneOSButton>(L, "FoneOSButton", FoneOSContainer_table, FoneOSButton_metatable);
	luaW_extend<FoneOSButton, FoneOSContainer>(L); // FoneOSButton inherits FoneOSContainer

	luaW_register<FoneOSScreen>(L, "FoneOSScreen", FoneOSContainer_table, FoneOSScreen_metatable);

	return 1;
}
