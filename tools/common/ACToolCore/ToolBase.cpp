#include "stdafx.h"
#include "ToolBase.h"
#include "ac_thread_guard.h"

namespace actools
{
	LogMFC* LogMFC::m_pInstance = 0;
	long LogMFC::m_nSingletonGuard = 0;
	bool LogMFC::m_bSingletonDestroyed = false;
	long LogMFC::m_nLogGuard = 0;

	LogMFC::LogMFC()
		: m_bIsClosed(true)
	{}

	LogMFC::~LogMFC()
	{
		m_bSingletonDestroyed = true;
		m_pInstance = 0;
	}

	void LogMFC::AddLog(const std::string& str)
	{
		acutils::ThreadGuard guard(&m_nLogGuard);
		m_vtLogCache.push_back(str);
	}

	int LogMFC::Update()
	{
		acutils::ThreadGuard guard(&m_nLogGuard);
		if(m_bIsClosed)
			return 0;

		if(!m_vtLogCache.empty())
		{
			std::string str = m_vtLogCache.front();
			m_vtLogCache.pop_front();
			::SendMessage(m_lpPrintHwnd, LB_ADDSTRING, 0, (LPARAM)str.c_str());
			::SendMessage(m_lpPrintHwnd, WM_VSCROLL, SB_PAGEDOWN, NULL);
		}

		return 0;
	}

	void LogMFC::OpenLog(HWND lpPrintHwnd)
	{
		acutils::ThreadGuard guard(&m_nLogGuard);
		if(!m_bIsClosed)
			return;

		m_bIsClosed = false;
		m_lpPrintHwnd = lpPrintHwnd;
	}

	void LogMFC::CloseLog()
	{
		acutils::ThreadGuard guard(&m_nLogGuard);
		if(m_bIsClosed)
			return;

		m_bIsClosed = true;
		m_lpPrintHwnd = NULL;
	}

	//-----------------------------------------------------------

	void LogFacilityMFC::Output(const std::string& str)
	{
		LogMFC::Instance().AddLog(str);
	}

	//-----------------------------------------------------------

	ToolBase* ToolBase::m_pInstance = NULL;

	ToolBase::ToolBase()
	{
		m_pInstance = this;
	}

	ToolBase::~ToolBase()
	{
		m_pInstance = NULL;
	}

	int ToolBase::InitTool(const std::string& filename, HWND lpPrintHwnd)
	{
		//初始化日志
		acutils::Log::Instance().AddFacility(new acutils::LogFacilityFile(filename));
		acutils::Log::Instance().AddFacility(new LogFacilityMFC());
		LogMFC::Instance().OpenLog(lpPrintHwnd);

		//lua相关初始化
		m_pLua = lua_open();
		luaL_openlibs(m_pLua);

		return 0;
	}

	int ToolBase::Update()
	{
		LogMFC::Instance().Update();
		return 0;
	}

	void ToolBase::DeInitTool()
	{
		LogMFC::Instance().CloseLog();

		lua_close(m_pLua);
	}
}