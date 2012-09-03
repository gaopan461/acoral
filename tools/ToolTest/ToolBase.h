#ifndef _TOOL_BASE_H_
#define _TOOL_BASE_H_

#include "ac_log.h"
#include <Windows.h>
#include <deque>

namespace actools
{
	class LogFacilityMFC : public acutils::LogFacilityBase
	{
	public:
		LogFacilityMFC(HWND lpPrintHwnd);
		LOG_INTERFACE ~LogFacilityMFC();
	public:
		LOG_INTERFACE void Output(const std::string& str);
		int Update();
	private:
		HWND m_lpPrintHwnd;
		std::deque<std::string> m_vtLogCache;
	};

	//-----------------------------------------------------------

	void LogInit(const std::string& filename, HWND lpPrintHwnd);
}

#endif//_TOOL_BASE_H_