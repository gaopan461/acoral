#include "ac_log.h"
#include <cstdarg>
#include <cstdio>

namespace acoral
{
	Log* Log::m_pInstance = 0;
	long Log::m_nSingletonGuard = 0;
	bool Log::m_bSingletonDestroyed = false;
	long Log::m_nLogGuard = 0;


	void LogConsole::printf(const char* format,...)
	{
		ThreadGuard guard(&m_nLogGuard);
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);
	}

	void LogConsole::dump(const void* buffer,size_t size)
	{
		ThreadGuard guard(&m_nLogGuard);
		return ;
	}

	void LogConsole::AddLog(int type, const char* format,...)
	{
		ThreadGuard guard(&m_nLogGuard);
		char* buf = NULL;

		va_list argptr;
		va_start(argptr, format);
		int nLen = _vscprintf(format,argptr);
		buf = new char[nLen + 1];
		vsprintf(buf,format,argptr);
		va_end(argptr);

		std::string str;
		switch(type)
		{
		case LOG_DEBUG:
			str += "[ DEBUG ] ";
			break;
		case LOG_ERROR:
			str += "[ ERROR ] ";
			break;
		case  LOG_WARNING:
			str += "[WARNING] ";
			break;
		case LOG_INFO:
			str += "[ INFO  ] ";
			break;
		}

		str += buf;
		std::cout << str;
		if(buf)
			delete [] buf;
	}
}