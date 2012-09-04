#pragma once

#include <afx.h>
#include <afxwin.h>
#include <string>
#include <map>
#include <vector>
#include "ac_lua.h"

#define USER_CTRL_ID_START				30000
//====================================================================================
#define DEFAULT_HEIGHT					20		//控件默认高度
#define DEFAULT_HEIGHT_INTER			5		//控件纵向间隔
#define MAIN_DEFAULT_WIDTH				100		//主控件默认宽度
#define PARAM_DEFAULT_WIDTH				100		//参数文本控件默认宽度
#define PARAM_DEFAULT_WIDTH1			100		//参数控件默认高度

//====================================================================================
// 捕获右键消息，用于弹出配置窗口
extern bool g_bIsPopMenu;

#define HANDLE_POP_CTRL \
	BOOL PreTranslateMessage(MSG* pMsg)\
	{\
		if (pMsg->message == WM_RBUTTONDOWN)\
		{\
			CWnd* wnd = WindowFromPoint(pMsg->pt);\
			CPopWindow popWin(IDD_POP, wnd);\
			CollectionMainConfsT::iterator iter = g_mapMainConfs.find(wnd->GetDlgCtrlID());\
			if(iter != g_mapMainConfs.end())\
			{\
				popWin.Show();\
				g_bIsPopMenu = false;\
				return TRUE;\
			}\
		}\
		else if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))\
		{\
			::TranslateMessage(pMsg);\
			::DispatchMessage(pMsg);\
			return TRUE;\
		}\
		return CDialog::PreTranslateMessage(pMsg);\
	}

//===================================================================================
//checkcombo和combobox配置
struct SComboItemConf
{
	CString m_strName;					//显示的名字
	int m_nValue;						//内部存储的值
	bool m_bChecked;					//是否默认被选中
	SComboItemConf()
	{
		m_strName = "";
		m_nValue = -1;
		m_bChecked = false;
	}
};

//参数控件配置
struct SPopParamConf
{
	CString m_strName;					//显示的参数名
	CString m_strCName;					//实际存储的参数名
	std::vector<SComboItemConf> m_vtComboConf;	//checkcombo和combobox的配置信息
	CString m_strCtrlType;				//控件类型，只能是（comcheckbox，combox，edit）
	CString m_strCast;					//保存时需要做的转换
	CString m_strDefault;				//默认值（用于edit）
	CString m_strNote;					//注释
	int m_nWidth;						//文本控件的宽（参数名）
	int m_nWidth1;						//控件的宽（参数值）
	SPopParamConf()
	{
		m_strName = "";
		m_strCName = "";
		m_vtComboConf.clear();
		m_strCtrlType = "";
		m_strCast = "";
		m_strDefault = "";
		m_strNote = "";
		m_nWidth = PARAM_DEFAULT_WIDTH;
		m_nWidth1 = PARAM_DEFAULT_WIDTH1;
	}
};

//主控件配置
struct SPopMainConf
{
	CString m_strName;						//显示值
	CString m_strCtrlType;					//控件类型
	int m_nValue;							//实际存储的值
	bool m_bChecked;						//默认是否选中
	bool m_bNewLine;						//是否不换行
	int m_nWidth;							//控件宽
	CString m_strNote;						//注释
	std::vector<SPopParamConf> m_vtParams;	//参数控件
	SPopMainConf()
	{
		m_strName = "";
		m_strCtrlType = "";
		m_nValue = -1;
		m_bChecked = false;
		m_bNewLine = true;
		m_nWidth = MAIN_DEFAULT_WIDTH;
		m_strNote = "";
		m_vtParams.clear();
	}
};

//一个配置
struct SPopConf
{
	CString m_strConfType;					//配置类型
	std::vector<SPopMainConf> m_vtMains;	//主配置列表
	SPopConf()
	{
		m_strConfType = "";
		m_vtMains.clear();
	}
};

//===================================================================================
//参数控件信息
struct SPopParam
{
	SPopParamConf* m_pPopParamConf;			//参数控件配置
	CWnd* m_pPopParamWnd;					//参数控件
};

//弹出的一行控件信息
struct SPopMain
{
	SPopMainConf* m_pPopMainConf;			//主控件配置
	CWnd* m_pPopMainWnd;					//主控件
	std::vector<SPopParam> m_vtPopParams;	//所有参数控件
};

// CPopWindow 对话框
class CPopWindow : public CDialog
{
	DECLARE_DYNAMIC(CPopWindow)

public:
	CPopWindow(int resID, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPopWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	void SetMain(CWnd* pWnd);
	BOOL Show();
	std::vector<SPopMain>& GetPopMains();
public:
	//创建所有配置控件
	int CreatePopControl();
private:
	//创建主控件
	int CreatePopMainControl(SPopMainConf& mainConf, DWORD nMainStyle);
	//创建参数控件
	int CreatePopParamControl(SPopParamConf& paramConf);
private:
	int m_nId;										//当前控件ID
	int m_nStartX;									//当前控件的起始
	int m_nStartY;									//当前控件的起始
	CWnd* m_pMainWnd;								//原始控件
	std::vector<CWnd*> m_vtWnds;					//全部控件
	std::vector<SPopMain> m_vtPopMains;				//全部控件
};

//===================================================================================
//存储所有原始控件和lua配置关联的映射表
//key为原始空的资源ID，value为原始控件的lua配置名
typedef std::map<int,CString> CollectionMainConfsT;
extern CollectionMainConfsT g_mapMainConfs;

//注册原始控件到映射表（将原始控件和lua配置关联）
bool REG_PROPERTY(int PID, const char* name);

//配置名字和它对应的弹出窗口
typedef std::map<CString,SPopConf> CollectionPopConfsT;
extern CollectionPopConfsT g_mapPopConfs;

//===================================================================================
//载入配置文件
bool LoadPopConfig(const std::string& name);

//配置数据转到主窗口的控件上
int PopToMain(CPopWindow* pPopWnd, CWnd* pMainWnd);

//主窗口控件的数据转到配置窗口上
int MainToPop(CWnd* pMainWnd, CPopWindow* pPopWnd);

//主窗口控件的数据转到数据库中
int MainToDB(CWnd* pMainWnd, lua_State* L, const std::string& strName);

//数据库中的数据转到主窗口控件
int DBToMain(lua_State* L, const std::string& strName, CWnd* pMainWnd);

//===================================================================================
void ConvertGBKToUTF8(CString& strGBK);
void ConvertUTF8ToGBK(CString& strUTF8);
