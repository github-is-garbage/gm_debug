#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/ModuleLoader.hpp>

int luaopen_debug(lua_State* L) // Manually reimplement because unresolved externals
{
	typedef int (*luaopen_debug_type) (lua_State* L);

	SourceSDK::ModuleLoader Loader_LuaShared("lua_shared");

	luaopen_debug_type luaopen_debug_fn = (luaopen_debug_type)Loader_LuaShared.GetSymbol("luaopen_debug");

	if (luaopen_debug_fn)
		luaopen_debug_fn(L);
}

LUA_FUNCTION(debug_getregistry)
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG);
	return 1;
}

GMOD_MODULE_OPEN()
{
	luaopen_debug(LUA->GetState());

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	{
		LUA->PushString("debug");
		LUA->RawGet(-2);
		{
			if (LUA->IsType(-1, GarrysMod::Lua::Type::TABLE))
			{
				LUA->PushString("getregistry");
				LUA->PushCFunction(debug_getregistry);
				LUA->RawSet(-3);
			}
		}
		LUA->Pop();
	}
	LUA->Pop();

	return 0;
}
