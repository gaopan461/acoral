#include "ac_log.h"
#include <cstdarg>
#include <cstdio>

namespace acutils
{
	Log* Log::m_pInstance = 0;
	long Log::m_nSingletonGuard = 0;
	bool Log::m_bSingletonDestroyed = false;
	long Log::m_nLogGuard = 0;

	//------------------------------------------------------

	LogEx::~LogEx()
	{
		for (std::vector<LogFacilityBase*>::iterator iter = m_vtLogFacilities.begin(); iter != m_vtLogFacilities.end(); iter++)
		{
			delete (*iter);
		}

		m_vtLogFacilities.clear();
	}

	void LogEx::AddLog(int type, const char* format,...)
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

		for(std::vector<LogFacilityBase*>::iterator iter = m_vtLogFacilities.begin(); iter != m_vtLogFacilities.end(); iter++)
			(*iter)->Output(str);
		
		if(buf)
			delete [] buf;
	}

	void LogEx::AddFacility(LogFacilityBase* pFacility)
	{
		ACCHECK(pFacility);

		ThreadGuard guard(&m_nLogGuard);
		m_vtLogFacilities.push_back(pFacility);
	}

	//------------------------------------------------------

	void LogFacilityConsole::Output(const std::string& str)
	{
		std::cout << str;
	}

	//------------------------------------------------------

	LogFacilityFile::LogFacilityFile(const std::string& filename)
		: m_objFileStream(filename.c_str(), std::ios::app|std::ios::out)
	{}

	LogFacilityFile::~LogFacilityFile()
	{
		m_objFileStream.close();
	}

	void LogFacilityFile::Output(const std::string &str)
	{
		m_objFileStream << str;
	}
}