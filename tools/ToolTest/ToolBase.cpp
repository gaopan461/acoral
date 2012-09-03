#include "stdafx.h"
#include "ToolBase.h"

namespace actools
{
	LogFacilityMFC::LogFacilityMFC(HWND lpPrintHwnd)
		: m_lpPrintHwnd(lpPrintHwnd)
	{}

	LogFacilityMFC::~LogFacilityMFC()
	{}

	void LogFacilityMFC::Output(const std::string& str)
	{
		m_vtLogCache.push_back(str);
	}

	int LogFacilityMFC::Update()
	{
		if(!m_vtLogCache.empty())
		{
			std::string str = m_vtLogCache.front();
			m_vtLogCache.pop_front();
			::SendMessage(m_lpPrintHwnd, LB_ADDSTRING, 0, (LPARAM)str.c_str());
			::SendMessage(m_lpPrintHwnd, WM_VSCROLL, SB_PAGEDOWN, NULL);
		}

		return 0;
	}

	//-----------------------------------------------------------

	void LogInit(const std::string& filename, HWND lpPrintHwnd)
	{
		acutils::Log::Instance().AddFacility(new acutils::LogFacilityFile(filename));
		acutils::Log::Instance().AddFacility(new LogFacilityMFC(lpPrintHwnd));
	}
}