#pragma once

#include <string>
#include <map>
#include <vector>
#include "ac_lua.h"

#define USER_CTRL_ID_START				30000
//====================================================================================
// 原始控件类型
#define ORIGIN_STYLE_EDITRADIO			0	//编辑框，单选
#define ORIGIN_STYLE_LISTBOX			1	//列表框，多选
#define ORIGIN_STYLE_EDITTUPLE			2	//编辑框，多选

// 弹出控件类型，主控件
#define POP_STYLE_MAIN_RADIO			0	//单选框
#define POP_STYLE_MAIN_CHECKBOX			1	//复选框

//弹出控件类型，参数控件
#define POP_STYLE_PARAM_EDIT			0	//编辑框
#define POP_STYLE_PARAM_COMBOX			1	//组合框
#define POP_STYLE_PARAM_COMCHECKBOX		2	//组合复选框

#define DEFAULT_HEIGHT					20		//控件默认高度
#define DEFAULT_HEIGHT_INTER			5		//控件纵向间隔
#define MAIN_DEFAULT_WIDTH				"100"	//主控件默认宽度
#define PARAM_DEFAULT_WIDTH				"100"	//参数文本控件默认宽度
#define PARAM_DEFAULT_WIDTH1			"100"	//参数控件默认高度

struct SOriginControlInfo;
struct SPopItem;
struct SPopControlMainInfo;
struct SPopControlParamInfo;

//====================================================================================
// 捕获右键消息，用于弹出配置窗口
extern bool g_bIsPopMenu;

#define HANDLE_POP_CTRL \
	BOOL PreTranslateMessage(MSG* pMsg)\
	{\
		if (pMsg->message == WM_RBUTTONDOWN)\
		{\
			CWnd* wnd = WindowFromPoint(pMsg->pt);\
			CPopWindow popWin(IDD_POP);\
			CollectionOriginInfosT::iterator iter = g_mapOriginInfos.find(wnd->GetDlgCtrlID());\
			if(iter != g_mapOriginInfos.end())\
			{\
				popWin.SetOrigin(wnd);\
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
// CPopWindow 对话框
class CPopWindow : public CDialog
{
	DECLARE_DYNAMIC(CPopWindow)

public:
	CPopWindow(int resID, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPopWindow();

// 对话框数据
	enum { IDD = IDD_POP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
public:
	void SetOrigin(CWnd* pWnd);
	BOOL Show();
	void CreatePopMain(SPopControlMainInfo* pMainInfo);
private:
	int m_nId;
	CWnd* m_pOriginWnd;								//原始控件
	std::vector<CWnd*> m_vtWnds;					//全部控件
	std::vector<SPopItem*> m_vtPopItems;			//全部控件
};

//弹出的一行控件信息
struct SPopItem
{
	SPopControlMainInfo* m_pMainInfo;
	CWnd* m_pMainWnd;
	std::vector<CWnd*> m_vtParamWnds;
	std::map<HWND,SPopControlParamInfo*> m_mapParamInfos;
};

//===================================================================================
//原始控件的信息
struct SOriginControlInfo
{
	std::string m_strName;		//关联的xml项的名字
	int m_nDlgStyle;			//原始控件的类型

	SOriginControlInfo()
	{
		m_strName = "";
		m_nDlgStyle = 0;
	}
};

//存储所有原始控件和xml关联的映射表
//key为原始空的资源ID，value为原始控件的信息
typedef std::map<int,SOriginControlInfo> CollectionOriginInfosT;
extern CollectionOriginInfosT g_mapOriginInfos;

//注册原始控件到映射表（将原始控件和xml关联）
bool REG_PROPERTY(int PID, const char* name, int dlgStyle);

//===================================================================================
//弹出控件，参数控件
struct SPopControlParamInfo
{
	std::string m_strName;			//显示的参数名
	std::string m_strCName;			//实际存储的参数名
	std::map<std::string,std::string> m_mapValue;	//显示的参数值:实际存储的参数值（只能是comcheckbox和combox）
	std::string m_strDlgStyle;		//控件类型，只能是（comcheckbox，combox，edit）
	std::string m_strReverse;		//保存时需要做的转换，只能是combox
	std::string m_strDefault;		//默认值（用于combox和edit，comcheckbox默认都不选）
	std::string m_strDescr;			//注释
	int m_nWidth,m_nHeight;			//文本控件的宽高（参数名）
	int m_nStartWidth,m_nStartHeight;//文本控件起始位置（参数名）
	int m_nWidth1,m_nHeight1;		//控件的宽高（参数值）

};
//弹出控件，主控件
struct SPopControlMainInfo
{
	std::string m_strName;			//显示值
	int m_nValue;					//实际存储的值
	std::string m_strDlgStyle;		//控件类型，只能是（radio和checkbox）
	std::string m_strDescr;			//注释
	bool m_bChecked;				//默认是否选中
	bool m_bNonewline;				//是否不换行
	int m_nWidth,m_nHeight;			//控件宽高
	int m_nStartWidth,m_nStartHeight;//控件起始位置
	std::vector<SPopControlParamInfo*> m_vtParams;	//参数控件
};

//一个弹出窗口的所有主控件
typedef std::vector<SPopControlMainInfo*> CollectionPopControlInfosT;
//xml名字和它对应的弹出窗口
typedef std::map<std::string,CollectionPopControlInfosT> CollectionPopInfosT;
extern CollectionPopInfosT g_mapPopInfos;

//===================================================================================
//载入xml文件
bool LoadPopConfig(std::string name);

#define DECLARE_NO(ISWRITETODB,DLGID,NAME) DeclareNo(L,ISWRITETODB,DLGID,NAME)
#define DECLARE_SOURCE_LISTBOX_DEFTYPE(ISWRITETODB,DLGID,NAME) DeclareSourceListBoxDefType(L,ISWRITETODB,DLGID,NAME)
#define DECLARE_SOURCE_LISTBOX_INT(ISWRITETODB,DLGID,NAME) DeclareSourceListBoxInt(L,ISWRITETODB,DLGID,NAME)
#define DECLARE_SOURCE_EDIT_INT(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceEditInt(L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_SOURCE_EDIT_STR(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceEditStr(L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_SOURCE_EDIT_DOUBLE(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceEditDouble(L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_SOURCE_EDIT_DEFTYPE(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceEditDefType(L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_SOURCE_EDIT_DEFTYPE_AND_PARAMS(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceEditDefTypeAndParams(ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_SOURCE_TUPLEEDIT_DEFTYPE(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceTupleEditDefType(L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_SOURCE_CHECKBOX(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareSourceCheckBox(ISWRITETODB,DLGID,NAME,DEFVAL)

//读/写ID
void DeclareNo(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//读/写列表框（自定义类型，需要在外部表示和内部表示间转换）
void DeclareSourceListBoxDefType(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//读/写列表框（整形）
void DeclareSourceListBoxInt(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//读/写编辑框（整形）
void DeclareSourceEditInt(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, long DEFVAL);
//读/写编辑框（字符串）
void DeclareSourceEditStr(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//读/写编辑框（浮点型）
void DeclareSourceEditDouble(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, float DEFVAL);
//读/写编辑框（自定义类型，需要在外部表示和内部表示间转换）
void DeclareSourceEditDefType(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//读/写编辑框（自定义类型，需要在外部表示和内部表示间转换，带参数）
void DeclareSourceEditDefTypeAndParams(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//读/写编辑框（自定义类型，需要在外部表示和内部表示间转换，包含多条记录）
void DeclareSourceTupleEditDefType(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//读/写复选框
void DeclareSourceCheckBox(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);

//===================================================================================
void ConvertGBKToUTF8(CString& strGBK);
void ConvertUTF8ToGBK(CString& strUTF8);
