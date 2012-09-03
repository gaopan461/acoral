#ifndef _AC_LOG_H_
#define _AC_LOG_H_

#include "ac_def.h"
#include "ac_thread_guard.h"
#include <cstdarg>
#include <fstream>

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

	//------------------------------------------------------

	/*
	 *	日志设施基类 
	 */
	class LogFacilityBase
	{
	public:
		LOG_INTERFACE ~LogFacilityBase(){}
	public:
		LOG_INTERFACE void Output(const std::string& str){}
	};

	class LogEx;

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


		//************************************
		// Method:    AddFacility 增加日志输出设施
		// FullName:  acutils::Log::AddFacility
		// Access:    public 
		// Returns:   LOG_INTERFACE void
		// Qualifier:
		// Parameter: LogFacilityBase * pFacility
		//************************************
		LOG_INTERFACE void AddFacility(LogFacilityBase* pFacility){}
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
	 *	扩展日志
	 */
	class LogEx : public Log
	{
	public:
		LOG_INTERFACE ~LogEx();
	public:
		LOG_INTERFACE void AddLog(int type, const char* format,...);
		LOG_INTERFACE void AddFacility(LogFacilityBase* pFacility);
	private:
		std::vector<LogFacilityBase*> m_vtLogFacilities;
	};

#ifdef AC_LOG
	inline void Log::CreateInstance()
	{
		ThreadGuard guard(&m_nSingletonGuard);
		if(m_pInstance)
			return;

		ACCHECK(!m_bSingletonDestroyed);
		static LogEx obj;
		m_pInstance = &obj;
	}
#endif //AC_LOG

	//------------------------------------------------------

	/*
	 *	输出到控制台 
	 */
	class LogFacilityConsole : public LogFacilityBase
	{
	public:
		LOG_INTERFACE void Output(const std::string& str);
	};

	//------------------------------------------------------

	/*
	 *	输出到文件 
	 */
	class LogFacilityFile : public LogFacilityBase
	{
	public:
		LogFacilityFile(const std::string& filename);
		LOG_INTERFACE ~LogFacilityFile();
	public:
		LOG_INTERFACE void Output(const std::string& str);
	private:
		std::fstream m_objFileStream;
	};

}

#endif //_AC_LOG_H_