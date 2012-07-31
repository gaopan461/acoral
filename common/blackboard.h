#ifndef _BLACK_BOARD_
#define _BLACK_BOARD_

#include <string>
#include <map>
#include <cassert>

class BoardVar
{
public:
	template<typename T>
	BoardVar(T* pData)
		: m_pData(pData)
	{}

	template<typename T>
	BoardVar(T data)
		: m_pData(new T(data))
	{}

	~BoardVar()
	{
		delete m_pData;
	}

	template<typename T>
	T* Get()
	{
		return reinterpret_cast<T*>(m_pData);
	}

	template<typename T>
	void Set(T* pData)
	{
		delete m_pData;
		m_pData = pData;
	}

	template<typename T>
	void Set(T data)
	{
		delete m_pData;
		m_pData = new T(data);
	}
private:
	void* m_pData;
};

class Board
{
public:
	~Board()
	{
		for(CollectionBoardVarsT::iterator iter = m_mapBoardVars.begin(); iter != m_mapBoardVars.end(); iter++)
			delete iter->second;
		m_mapBoardVars.clear();
	}
	void Add(const std::string& name, BoardVar* var)
	{
		assert(m_mapBoardVars.find(name) == m_mapBoardVars.end());
		m_mapBoardVars.insert(std::make_pair(name, var));
	}

	void Del(const std::string& name)
	{
		if(m_mapBoardVars.find(name) != m_mapBoardVars.end())
		{
			delete m_mapBoardVars[name];
			m_mapBoardVars.erase(name);
		}
	}

	void Set(const std::string& name, BoardVar* var)
	{
		if(m_mapBoardVars.find(name) != m_mapBoardVars.end())
			delete m_mapBoardVars[name];

		m_mapBoardVars[name] = var;
	}

	BoardVar* Get(const std::string& name)
	{
		if(m_mapBoardVars.find(name) != m_mapBoardVars.end())
			return m_mapBoardVars[name];

		return NULL;
	}
protected:
	typedef std::map<std::string, BoardVar*> CollectionBoardVarsT;
private:
	CollectionBoardVarsT m_mapBoardVars;
};

#endif	//_BLACK_BOARD_