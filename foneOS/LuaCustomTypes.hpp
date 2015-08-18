#pragma once
#include "stdafx.h"

#include "lua.h"
#include "luawrapper.hpp"
#include "luawrapperutil.hpp"

template<>
struct luaU_Impl<FoneOSString>
{
	static FoneOSString luaU_check(lua_State* L, int index)
	{
		return Utils::CharArrayToFoneOSString((char *)luaL_checkstring(L, index));
	}

	static FoneOSString luaU_to(lua_State* L, int index)
	{
		return Utils::CharArrayToFoneOSString((char *)lua_tostring(L, index));
	}

	static void luaU_push(lua_State* L, const FoneOSString& val)
	{
		 lua_pushstring(L, Utils::FoneOSStringToCharArray(val));
	}
};
