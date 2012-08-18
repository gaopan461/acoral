#include "ac_lua.h"

namespace acutils
{
	LuaTableSave::LuaTableSave(lua_State* L, const char* tablename, const char* filename, int maxLayer)
	{
		ACCHECK(L);
		m_pLua = L;
		ACCHECK(tablename);
		ACCHECK(filename);
		m_objOut.open(filename, std::ios::out);
		ACCHECK(!m_objOut.bad());
		m_nLayer = 0;
		m_nMaxLayer = maxLayer > 0 ? maxLayer : 0;

		m_objOut << tablename << " = ";
	}

	LuaTableSave::~LuaTableSave()
	{
		m_objOut.close();
	}

	int LuaTableSave::Save()
	{
		ACCHECK(lua_istable(m_pLua, -1));
		m_objOut << "{";
		bool isEmpty = true;
		lua_pushnil(m_pLua);
		m_nLayer++;
		while(lua_next(m_pLua, -2) != 0)
		{
			isEmpty = false;
			PrintTab();

			//存储KEY
			if(lua_type(m_pLua,-2) == LUA_TNUMBER)
				m_objOut << "[" << lua_tonumber(m_pLua, -2) << "]" << " = ";
			else if(lua_type(m_pLua, -2) == LUA_TSTRING)
				m_objOut << lua_tostring(m_pLua, -2) << " = ";
			else
			{
				std::cout << "Unknown key type" << std::endl;
				ACCHECK(false);
			}

			//存储VALUE
			if(lua_type(m_pLua, -1) == LUA_TNUMBER)
				m_objOut << lua_tonumber(m_pLua, -1) << ",";
			else if(lua_type(m_pLua, -1) == LUA_TSTRING)
				m_objOut << "\"" << lua_tostring(m_pLua, -1) << "\"" << ",";
			else if(lua_type(m_pLua, -1) == LUA_TBOOLEAN)
			{
				int bval = lua_toboolean(m_pLua, -1);
				std::string strVal = bval ? "true" : "false";
				m_objOut << strVal << ",";
			}
			else if(lua_type(m_pLua, -1) == LUA_TTABLE)
			{
				Save();
			}
			else
			{
				std::cout << "Unknown value type" << std::endl;
				ACCHECK(false);
			}

			lua_pop(m_pLua, 1);
		}

		m_nLayer--;

		//层数未达到最大且table不为空，table结束处换行
		if((m_nLayer + 1 <= m_nMaxLayer) && (!isEmpty))
			PrintTab();

		m_objOut << "}";

		//不是第一层
		if(m_nLayer)
			m_objOut << ",";

		return 0;
	}

	void LuaTableSave::PrintTab()
	{
		if(m_nLayer > m_nMaxLayer)
			return;

		m_objOut << "\n";
		for(int i = 0; i < m_nLayer; i++)
			m_objOut << "\t";
	}

	//-------------------------------------------------------------------------

	int LuaUtils::SaveTableToFile(lua_State* L, const char* tablename, const char* filename, int maxLayer /* = 3 */)
	{
		LuaTableSave luaTableSave(L, tablename, filename, maxLayer);
		return luaTableSave.Save();
	}
}