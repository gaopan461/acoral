#ifndef _AC_LUA_H_
#define _AC_LUA_H_

#include "lua.hpp"
#include "ac_def.h"
#include <fstream>

namespace acutils
{
	/*
	 *	�洢lua table
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
	 *	lua��ص�ͨ�ò���
	 */
	class LuaUtils
	{
	public:
		//************************************
		// Method:    SaveTableToFile ��lua��table�����ļ���(���ԭ���ļ��е�����)
		// FullName:  acutils::LuaUtils::SaveTableToFile
		// Access:    public static 
		// Returns:   int
		// Qualifier:
		// Parameter: lua_State * L
		// Parameter: const char * tablename ��Ҫ�洢�ı���
		// Parameter: const char * filename ��Ҫ�洢�����ļ���
		// Parameter: int maxLayer table���ļ���չ���������
		//************************************
		static int SaveTableToFile(lua_State* L, const char* tablename, const char* filename, int maxLayer = 3);
	};
}

#endif //_AC_LUA_H_