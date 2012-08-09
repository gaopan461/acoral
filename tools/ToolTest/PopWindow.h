#pragma once

#include <string>
#include <map>
#include <vector>

//====================================================================================
// ԭʼ�ؼ�����
#define ORIGIN_STYLE_EDITRADIO			0	//�༭�򣬵�ѡ
#define ORIGIN_STYLE_LISTBOX			1	//�б�򣬶�ѡ
#define ORIGIN_STYLE_EDITTUPLE			2	//�༭�򣬶�ѡ

// �����ؼ����ͣ����ؼ�
#define POP_STYLE_MAIN_RADIO			0	//��ѡ��
#define POP_STYLE_MAIN_CHECKBOX			1	//��ѡ��

//�����ؼ����ͣ������ؼ�
#define POP_STYLE_PARAM_EDIT			0	//�༭��
#define POP_STYLE_PARAM_COMBOX			1	//��Ͽ�
#define POP_STYLE_PARAM_COMCHECKBOX		2	//��ϸ�ѡ��

struct SOriginControlInfo;
struct SPopItem;
struct SPopControlMainInfo;
struct SPopControlParamInfo;

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
public:
	void SetOrigin(CWnd* pWnd);
	BOOL Show();
	void LoadConfig();
private:
	CWnd* m_pOriginWnd;								//ԭʼ�ؼ�
	std::vector<CWnd*> m_vtWnds;					//ȫ���ؼ�
	std::vector<SPopItem*> m_vtPopItems;			//ȫ���ؼ�
};

//������һ�пؼ���Ϣ
struct SPopItem
{
	SPopControlMainInfo* m_pMainInfo;
	CWnd* m_pMainWnd;
	std::vector<CWnd*> m_vtParamWnds;
	std::map<HWND,SPopControlParamInfo*> m_mapParamInfos;
};

//===================================================================================
//ԭʼ�ؼ�����Ϣ
struct SOriginControlInfo
{
	std::string m_strName;		//������xml�������
	int m_nDlgStyle;			//ԭʼ�ؼ�������

	SOriginControlInfo()
	{
		m_strName = "";
		m_nDlgStyle = 0;
	}
};

//�洢����ԭʼ�ؼ���xml������ӳ���
//keyΪԭʼ�յ���ԴID��valueΪԭʼ�ؼ�����Ϣ
typedef std::map<int,SOriginControlInfo> CollectionOriginInfosT;
extern CollectionOriginInfosT g_mapOriginInfos;

//ע��ԭʼ�ؼ���ӳ�����ԭʼ�ؼ���xml������
bool REG_PROPERTY(int PID, const char* name, int dlgStyle);

//===================================================================================
//�����ؼ��������ؼ�
struct SPopControlParamInfo
{
	std::string m_strName;			//��ʾ�Ĳ�����
	std::string m_strCName;			//ʵ�ʴ洢�Ĳ�����
	std::map<std::string,std::string> m_mapValue;	//��ʾ�Ĳ���ֵ:ʵ�ʴ洢�Ĳ���ֵ��ֻ����comcheckbox��combox��
	std::string m_strDlgStyle;		//�ؼ����ͣ�ֻ���ǣ�comcheckbox��combox��edit��
	std::string m_strReverse;		//����ʱ��Ҫ����ת����ֻ����combox
	std::string m_strDefault;		//Ĭ��ֵ������combox��edit��comcheckboxĬ�϶���ѡ��
	std::string m_strDescr;			//ע��
	int m_nWidth,m_nHeight;			//�ı��ؼ��Ŀ�ߣ���������
	int m_nStartWidth,m_nStartHeight;//�ı��ؼ���ʼλ�ã���������
	int m_nWidth1,m_nHeight1;		//�ؼ��Ŀ�ߣ�����ֵ��

};
//�����ؼ������ؼ�
struct SPopControlMainInfo
{
	std::string m_strName;			//��ʾֵ
	int m_nValue;					//ʵ�ʴ洢��ֵ
	std::string m_strDlgStyle;		//�ؼ����ͣ�ֻ���ǣ�radio��checkbox��
	std::string m_strDescr;			//ע��
	bool m_bChecked;				//Ĭ���Ƿ�ѡ��
	bool m_bNonewline;				//�Ƿ񲻻���
	int m_nWidth,m_nHeight;			//�ؼ����
	int m_nStartWidth,m_nStartHeight;//�ؼ���ʼλ��
	std::vector<SPopControlParamInfo*> m_vtParams;	//�����ؼ�
};

//һ���������ڵ��������ؼ�
typedef std::vector<SPopControlMainInfo*> CollectionPopControlInfosT;
//xml���ֺ�����Ӧ�ĵ�������
typedef std::map<std::string,CollectionPopControlInfosT> CollectionPopInfosT;
extern CollectionPopInfosT g_mapPopInfos;

//===================================================================================
//����xml�ļ�
bool LoadPopConfig(std::string name);

//��/дID
void DECLARE_NO(bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�б���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת����
void DECLARE_SOURCE_LISTBOX_DEFTYPE(bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�б�����Σ�
void DECLARE_SOURCE_LISTBOX_INT(bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�༭�����Σ�
void DECLARE_SOURCE_EDIT_INT(bool ISWRITETODB, int DLGID, const char* NAME, long DEFVAL);
//��/д�༭���ַ�����
void DECLARE_SOURCE_EDIT_STR(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д�༭�򣨸����ͣ�
void DECLARE_SOURCE_EDIT_DOUBLE(bool ISWRITETODB, int DLGID, const char* NAME, float DEFVAL);
//��/д�༭���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת����
void DECLARE_SOURCE_EDIT_DEFTYPE(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д�༭���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת������������
void DECLARE_SOURCE_EDIT_DEFTYPE_AND_PARAMS(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д�༭���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת��������������¼��
void DECLARE_SOURCE_TUPLEEDIT_DEFTYPE(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д��ѡ��
void DECLARE_SOURCE_CHECKBOX(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
