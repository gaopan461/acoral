#pragma once

#include <string>
#include <map>
#include <vector>
#include "ac_lua.h"

#define USER_CTRL_ID_START				30000
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

#define DEFAULT_HEIGHT					20		//�ؼ�Ĭ�ϸ߶�
#define DEFAULT_HEIGHT_INTER			5		//�ؼ�������
#define MAIN_DEFAULT_WIDTH				"100"	//���ؼ�Ĭ�Ͽ��
#define PARAM_DEFAULT_WIDTH				"100"	//�����ı��ؼ�Ĭ�Ͽ��
#define PARAM_DEFAULT_WIDTH1			"100"	//�����ؼ�Ĭ�ϸ߶�

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
typedef std::vector<CString> CollectionParamValuesT;
typedef std::vector<std::pair<CString,CollectionParamValuesT>> CollectionParamTextsT;
typedef std::map<CString,CollectionParamTextsT> CollectionMainTextsT;

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
	void SetOrigin(CWnd* pWnd);
	BOOL Show();
private:
	void CreatePopMain(SPopControlMainInfo* pMainInfo);
	int UpdateOriginToPop();
	int WritePopToOrigin();
private:
	int OriginToMidData(CollectionMainTextsT& mapData);
	int PopToMidData(std::vector<CString>& vtData);
public:
	static int OriginToMidData(CWnd* pOriginWnd, CollectionMainTextsT& mapData);
private:
	int m_nId;
	CWnd* m_pOriginWnd;								//ԭʼ�ؼ�
	std::vector<CWnd*> m_vtWnds;					//ȫ���ؼ�
	std::vector<SPopItem*> m_vtPopItems;			//ȫ���ؼ�
};

//������һ�пؼ���Ϣ
struct SPopItem
{
	SPopControlMainInfo* m_pMainInfo;
	CWnd* m_pMainWnd;
	std::vector<std::pair<CWnd*,SPopControlParamInfo*>> m_vtParamInfos;
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
void UnloadPopConfig();

#define DECLARE_NO(ISWRITETODB,DLGID,NAME) DeclareNo(this, L,ISWRITETODB,DLGID,NAME)
#define DECLARE_LISTBOX_DEFTYPE(ISWRITETODB,DLGID,NAME) DeclareListBoxDefType(this, L,ISWRITETODB,DLGID,NAME)
#define DECLARE_LISTBOX_DEFTYPE_AND_PARAMS(ISWRITETODB,DLGID,NAME) DeclareListBoxDefTypeAndParams(this, L,ISWRITETODB,DLGID,NAME)
#define DECLARE_LISTBOX_INT(ISWRITETODB,DLGID,NAME) DeclareListBoxInt(this, L,ISWRITETODB,DLGID,NAME)
#define DECLARE_EDIT_INT(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareEditInt(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_EDIT_STR(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareEditStr(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_EDIT_DOUBLE(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareEditDouble(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_EDIT_DEFTYPE(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareEditDefType(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_EDIT_DEFTYPE_AND_PARAMS(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareEditDefTypeAndParams(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_TUPLEEDIT_DEFTYPE(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareTupleEditDefType(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)
#define DECLARE_CHECKBOX(ISWRITETODB,DLGID,NAME,DEFVAL) DeclareCheckBox(this, L,ISWRITETODB,DLGID,NAME,DEFVAL)

//��/дID
void DeclareNo(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�б���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת����
void DeclareListBoxDefType(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�б���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת������������
void DeclareListBoxDefTypeAndParams(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�б�����Σ�
void DeclareListBoxInt(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME);
//��/д�༭�����Σ�
void DeclareEditInt(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, long DEFVAL);
//��/д�༭���ַ�����
void DeclareEditStr(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д�༭�򣨸����ͣ�
void DeclareEditDouble(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, float DEFVAL);
//��/д�༭���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת����
void DeclareEditDefType(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д�༭���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת������������
void DeclareEditDefTypeAndParams(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д�༭���Զ������ͣ���Ҫ���ⲿ��ʾ���ڲ���ʾ��ת��������������¼��
void DeclareTupleEditDefType(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);
//��/д��ѡ��
void DeclareCheckBox(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL);

//===================================================================================
void ConvertGBKToUTF8(CString& strGBK);
void ConvertUTF8ToGBK(CString& strUTF8);
