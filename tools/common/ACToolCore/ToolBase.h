#ifndef _TOOL_BASE_H_
#define _TOOL_BASE_H_

#include "ac_log.h"
#include <Windows.h>
#include <deque>
#include "ac_thread_guard.h"

namespace actools
{
	/*
	 *	MFC日志，实际输出日志的地方，日志被输出到LISTBOX
	 *  每次Update时从缓冲区取出一条日志添加LISTBOX末尾
	 */
	class LogMFC
	{
		static void CreateInstance();
		static LogMFC* m_pInstance;
		static long m_nSingletonGuard;
		static bool m_bSingletonDestroyed;
	protected:
		static long m_nLogGuard;
	public:
		static LogMFC& Instance();
		LogMFC();
		virtual ~LogMFC();
	public:
		/*
		 *	先将日志记录到缓冲区
		 */
		void AddLog(const std::string& str);
		
		/*
		 *	将缓冲区日志更新到LISTBOX（实际输出日志的地方）
		 */
		int Update();

		/*
		 *	打开日志输出
		 */
		void OpenLog(HWND lpPrintHwnd);

		/*
		 *	关闭日志输出
		 */
		void CloseLog();
	private:
		HWND m_lpPrintHwnd;						//输入日志的窗口（LISTBOX）
		std::deque<std::string> m_vtLogCache;	//日志缓冲区
		bool m_bIsClosed;						//标记日志是否打开输出
	};

	inline LogMFC& LogMFC::Instance()
	{
		if(!m_pInstance)
			CreateInstance();

		return *m_pInstance;
	}

	inline void LogMFC::CreateInstance()
	{
		acutils::ThreadGuard guard(&m_nSingletonGuard);
		if(m_pInstance)
			return;

		ACCHECK(!m_bSingletonDestroyed);
		static LogMFC obj;
		m_pInstance = &obj;
	}

	//-----------------------------------------------------------

	/*
	 *	MFC日志设施
	 */
	class LogFacilityMFC : public acutils::LogFacilityBase
	{
	public:
		LogFacilityMFC(){}
		LOG_INTERFACE ~LogFacilityMFC(){}
	public:
		LOG_INTERFACE void Output(const std::string& str);
	};

	//-----------------------------------------------------------

	class ToolBase
	{
	public:
		ToolBase();
		virtual ~ToolBase();
	public:
		virtual int InitTool(const std::string& filename, HWND lpPrintHwnd);
		virtual int Update();
		virtual void DeInitTool();
	};
}

#endif//_TOOL_BASE_H_