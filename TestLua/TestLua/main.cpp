#include "ac_lua.h"

int main()
{
	lua_State* L = lua_open();
	luaL_openlibs(L);
	luaL_dofile(L, "gaopan.lua");
	lua_getglobal(L, "datas");
	acutils::LuaUtils::SaveTableToFile(L, "datass", "gaopan1.lua");
	return 0;
}