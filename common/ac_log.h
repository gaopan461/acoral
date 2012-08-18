#ifndef _AC_LOG_H_
#define _AC_LOG_H_

#include "ac_def.h"
#include "ac_thread_guard.h"
#include <cstdarg>

#ifdef AC_LOG
#define LOG_INTERFACE virtual
#else
#define LOG_INTERFACE
#endif //AC_LOG

#define DEBUG_MSG(fmt,...) acutils::Log::Instance().AddLog(acutils::LOG_DEBUG,fmt,__VA_ARGS__)
#define ERROR_MSG(fmt,...) acutils::Log::Instance().AddLog(acutils::LOG_ERROR,fmt,__VA_ARGS__)
#define WARN_MSG(fmt,...) acutils::Log::Instance().AddLog(acutils::LOG_WARNING,fmt,__VA_ARGS__)
#define INFO_MSG(fmt,...) acutils::Log::Instance().AddLog(acutils::LOG_INFO,fmt,__VA_ARGS__)

namespace acutils
{
	/*
	 *	日志级别 
	 */
	enum ELogType
	{
		LOG_DEBUG,				//调试
		LOG_ERROR,				//错误
		LOG_WARNING,			//警告
		LOG_INFO,				//普通
	};

	class LogConsole;

	/*
	 *	日志基类
	 */
	class Log
	{
		static void CreateInstance();
		static Log* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroyed;
	protected:
		static long m_nLogGuard;
	public:
		static Log& Instance();
		LOG_INTERFACE ~Log()
		{
			m_bSingletonDestroyed = true;
			m_pInstance = 0;
		}
		LOG_INTERFACE void printf(const char* format,...){}
		LOG_INTERFACE void dump(const void* buffer,size_t size){}

		//************************************
		// Method:    AddLog 输入日志
		// FullName:  acutils::Log::AddLog
		// Access:    public 
		// Returns:   LOG_INTERFACE void
		// Qualifier:
		// Parameter: int type 日志级别
		// Parameter: const char * format 要输出的内容
		// Parameter: ...
		//************************************
		LOG_INTERFACE void AddLog(int type, const char* format,...){}
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

	/*
	 *	控制台日志
	 */
	class LogConsole : public Log
	{
	public:
		LOG_INTERFACE void printf(const char* format,...);
		LOG_INTERFACE void dump(const void* buffer,size_t size);
		LOG_INTERFACE void AddLog(int type, const char* format,...);
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