#ifndef _AC_LUA_H_
#define _AC_LUA_H_

#include "lua.hpp"
#include "ac_def.h"
#include <fstream>

namespace acutils
{
	/*
	 *	存储lua table
	 */
	class LuaTableSave
	{
	public:
		LuaTableSave(lua_State* L, const char* tablename, const char* filename, int maxLayer);
		virtual ~LuaTableSave();
	public:
		int Save();
	private:
		void PrintTab();
	private:
		std::ofstream m_objOut;
		lua_State* m_pLua;
		int m_nLayer;
		int m_nMaxLayer;
	};

	/*
	 *	lua相关的通用操作
	 */
	class LuaUtils
	{
	public:
		//************************************
		// Method:    SaveTableToFile 将lua的table存入文件中(清空原有文件中的内容)
		// FullName:  acutils::LuaUtils::SaveTableToFile
		// Access:    public static 
		// Returns:   int
		// Qualifier:
		// Parameter: lua_State * L
		// Parameter: const char * tablename 将要存储的表名
		// Parameter: const char * filename 将要存储到的文件名
		// Parameter: int maxLayer table在文件中展开的最大层次
		//************************************
		static int SaveTableToFile(lua_State* L, const char* tablename, const char* filename, int maxLayer = 3);
	};
}

#endif //_AC_LUA_H_