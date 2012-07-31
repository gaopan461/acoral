#include "ac_log.h"
#include <cstdarg>
#include <cstdio>

namespace acoral
{
	Log* Log::m_pInstance = 0;
	long Log::m_nSingletonGuard = 0;
	bool Log::m_bSingletonDestroyed = false;


	void LogConsole::printf(const char* format,...)
	{
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);
	}

	void LogConsole::dump(const void* buffer,size_t size)
	{
		return ;
	}
}