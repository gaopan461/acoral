#pragma once

#include <string>
#include <map>
#include <vector>

//====================================================================================
#define ORIGIN_STYLE_NORMAL				0
#define ORIGIN_STYLE_LISTBOX			1
#define ORIGIN_STYLE_EDITTUPLEBOX		2

struct SCustonPropertyOriginInfo;
struct SPopPropertyInfo;

//====================================================================================
extern bool g_bIsPopMenu;

#define HANDLE_POP_CTRL \
	BOOL PreTranslateMessage(MSG* pMsg)\
	{\
		if (pMsg->message == WM_RBUTTONDOWN)\
		{\
			CWnd* wnd = WindowFromPoint(pMsg->pt);\
			CPopWindow popWin(IDD_POP);\
			CollectionPropertyInfosT::iterator iter = g_mapPropertyMaps.find(wnd->GetDlgCtrlID());\
			if(iter != g_mapPropertyMaps.end())\
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
	void SetOrigin(CWnd* pWnd);
	BOOL Show();
	afx_msg void OnClose();
private:
	CWnd* m_pOriginWnd;
	std::vector<CWnd*> m_vtWnds;
	std::vector<SPopPropertyInfo*> m_vtPropertyInfos;
};

struct SPopPropertyInfo
{
	SCustonPropertyOriginInfo* m_pProperty;
	CWnd* m_pMainWnd;
	std::vector<CWnd*> m_vtParamWnds;
	std::map<HWND,SCustonPropertyOriginInfo*> m_mapWndToInfos;
};

//===================================================================================
struct SCustonPropertyOriginInfo
{
	std::string name,sourceName;
	int dlgStyle;

	SCustonPropertyOriginInfo()
	{
		name = "";
		sourceName = "";
		dlgStyle = 0;
	}
};

typedef std::map<int,SCustonPropertyOriginInfo> CollectionPropertyInfosT;
extern CollectionPropertyInfosT g_mapPropertyMaps;

bool REG_PROPERTY(int PID, const char* name, int dlgStyle, const char* sourcename);
