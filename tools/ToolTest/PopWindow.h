#pragma once

#include <string>
#include <map>
#include <vector>
#include "ac_lua.h"

#define USER_CTRL_ID_START				30000
//====================================================================================
#define DEFAULT_HEIGHT					20		//�ؼ�Ĭ�ϸ߶�
#define DEFAULT_HEIGHT_INTER			5		//�ؼ�������
#define MAIN_DEFAULT_WIDTH				100		//���ؼ�Ĭ�Ͽ��
#define PARAM_DEFAULT_WIDTH				100		//�����ı��ؼ�Ĭ�Ͽ��
#define PARAM_DEFAULT_WIDTH1			100		//�����ؼ�Ĭ�ϸ߶�

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
			CollectionMainConfsT::iterator iter = g_mapMainConfs.find(wnd->GetDlgCtrlID());\
			if(iter != g_mapMainConfs.end())\
			{\
				popWin.SetMain(wnd);\
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
//checkcombo��combobox����
struct SComboItemConf
{
	CString m_strName;					//��ʾ������
	int m_nValue;						//�ڲ��洢��ֵ
	bool m_bChecked;					//�Ƿ�Ĭ�ϱ�ѡ��
	SComboItemConf()
	{
		m_strName = "";
		m_nValue = -1;
		m_bChecked = false;
	}
};

//�����ؼ�����
struct SPopParamConf
{
	CString m_strName;					//��ʾ�Ĳ�����
	CString m_strCName;					//ʵ�ʴ洢�Ĳ�����
	std::vector<SComboItemConf> m_vtComboConf;	//checkcombo��combobox��������Ϣ
	CString m_strCtrlType;				//�ؼ����ͣ�ֻ���ǣ�comcheckbox��combox��edit��
	CString m_strCast;					//����ʱ��Ҫ����ת��
	CString m_strDefault;				//Ĭ��ֵ������edit��
	CString m_strNote;					//ע��
	int m_nWidth;						//�ı��ؼ��Ŀ���������
	int m_nWidth1;						//�ؼ��Ŀ�����ֵ��
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

//���ؼ�����
struct SPopMainConf
{
	CString m_strName;						//��ʾֵ
	CString m_strCtrlType;					//�ؼ�����
	int m_nValue;							//ʵ�ʴ洢��ֵ
	bool m_bChecked;						//Ĭ���Ƿ�ѡ��
	bool m_bNewLine;						//�Ƿ񲻻���
	int m_nWidth;							//�ؼ���
	CString m_strNote;						//ע��
	std::vector<SPopParamConf> m_vtParams;	//�����ؼ�
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

//һ������
struct SPopConf
{
	CString m_strConfType;					//��������
	std::vector<SPopMainConf> m_vtMains;	//�������б�
	SPopConf()
	{
		m_strConfType = "";
		m_vtMains.clear();
	}
};

//===================================================================================
//�����ؼ���Ϣ
struct SPopParam
{
	SPopParamConf* m_pPopParamConf;			//�����ؼ�����
	CWnd* m_pPopParamWnd;					//�����ؼ�
};

//������һ�пؼ���Ϣ
struct SPopMain
{
	SPopMainConf* m_pPopMainConf;			//���ؼ�����
	CWnd* m_pPopMainWnd;					//���ؼ�
	std::vector<SPopParam> m_vtPopParams;	//���в����ؼ�
};

// CPopWindow �Ի���
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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	void SetMain(CWnd* pWnd);
	BOOL Show();
private:
	int m_nId;
	CWnd* m_pMainWnd;								//ԭʼ�ؼ�
	std::vector<CWnd*> m_vtWnds;					//ȫ���ؼ�
	std::vector<SPopMain> m_vtPopMains;			//ȫ���ؼ�
};

//===================================================================================
//�洢����ԭʼ�ؼ���xml������ӳ���
//keyΪԭʼ�յ���ԴID��valueΪԭʼ�ؼ���xml������
typedef std::map<int,CString> CollectionMainConfsT;
extern CollectionMainConfsT g_mapMainConfs;

//ע��ԭʼ�ؼ���ӳ�����ԭʼ�ؼ���xml������
bool REG_PROPERTY(int PID, const char* name);

//xml���ֺ�����Ӧ�ĵ�������
typedef std::map<CString,SPopConf> CollectionPopConfsT;
extern CollectionPopConfsT g_mapPopConfs;

//===================================================================================
//����xml�ļ�
bool LoadPopConfig(const std::string& name);

//===================================================================================
void ConvertGBKToUTF8(CString& strGBK);
void ConvertUTF8ToGBK(CString& strUTF8);
