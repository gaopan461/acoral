#pragma once

#include <string>
#include <map>
#include <vector>

//====================================================================================
// 可弹出配置窗口的控件类型
#define ORIGIN_STYLE_EDITRADIO			0	//编辑框，单选
#define ORIGIN_STYLE_LISTBOX			1	//列表框，多选
#define ORIGIN_STYLE_EDITTUPLE			2	//编辑框，多选

struct SCustonPropertyOriginInfo;
struct SPopPropertyInfo;

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

struct SRadioItem
{
	std::string m_strName;
	int m_nValue;
};

struct SRadioInfo
{
	std::vector<SRadioItem> m_vtRadioInfo;
};

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
	void LoadConfig();
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
	std::string name;
	int dlgStyle;

	SCustonPropertyOriginInfo()
	{
		name = "";
		dlgStyle = 0;
	}
};

typedef std::map<int,SCustonPropertyOriginInfo> CollectionPropertyInfosT;
extern CollectionPropertyInfosT g_mapPropertyMaps;

bool REG_PROPERTY(int PID, const char* name, int dlgStyle);
