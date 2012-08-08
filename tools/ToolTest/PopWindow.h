#pragma once

#include <string>
#include <map>
#include <vector>

//====================================================================================
// �ɵ������ô��ڵĿؼ�����
#define ORIGIN_STYLE_EDITRADIO			0	//�༭�򣬵�ѡ
#define ORIGIN_STYLE_LISTBOX			1	//�б�򣬶�ѡ
#define ORIGIN_STYLE_EDITTUPLE			2	//�༭�򣬶�ѡ

struct SCustonPropertyOriginInfo;
struct SPopPropertyInfo;

//====================================================================================
// �����Ҽ���Ϣ�����ڵ������ô���
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
// CPopWindow �Ի���

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
	CPopWindow(int resID, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPopWindow();

// �Ի�������
	enum { IDD = IDD_POP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
