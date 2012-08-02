#ifndef _AC_LOG_H_
#define _AC_LOG_H_

#include "ac_def.h"
#include "ac_thread_guard.h"

#ifdef AC_LOG
#define LOG_INTERFACE virtual
#else
#define LOG_INTERFACE
#endif //AC_LOG

namespace acoral
{

	class LogConsole;

	class Log
	{
		static void CreateInstance();
		static Log* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroyed;
	public:
		static Log& Instance();
		LOG_INTERFACE ~Log()
		{
			m_bSingletonDestroyed = true;
			m_pInstance = 0;
		}
		LOG_INTERFACE void printf(const char* format,...){}
		LOG_INTERFACE void dump(const void* buffer,size_t size){}
	};

#ifdef AC_LOG
	inline Log& Log::Instance()
	{
		if(!m_pInstance)
			CreateInstance();

		return *m_pInstance;
	}
#else
	inline Log& Log::Instance()
	{
		return *reinterpret_cast<Log*>(0);
	}
#endif //AC_LOG

	//------------------------------------------------------

	class LogConsole : public Log
	{
	public:
		LOG_INTERFACE void printf(const char* format,...);
		LOG_INTERFACE void dump(const void* buffer,size_t size);
	};

#ifdef AC_LOG
	inline void Log::CreateInstance()
	{
		ThreadGuard guard(&m_nSingletonGuard);
		if(m_pInstance)
			return;

		ACCHECK(!m_bSingletonDestroyed);
		static LogConsole obj;
		m_pInstance = &obj;
	}
#endif //AC_LOG

}

#endif //_AC_LOG_H_