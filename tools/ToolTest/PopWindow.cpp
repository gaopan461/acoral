// PopWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ToolTest.h"
#include "PopWindow.h"
#include "CheckComboBox.h"

//===================================================================================
bool g_bIsPopMenu = true;

//===================================================================================
// CPopWindow �Ի���

IMPLEMENT_DYNAMIC(CPopWindow, CDialog)

CPopWindow::CPopWindow(int resID, CWnd* pParent /*=NULL*/)
	: CDialog(resID, pParent)
	, m_nId(USER_CTRL_ID_START)
	, m_pMainWnd(pParent)
	, m_nStartX(0)
	, m_nStartY(0)
{

}

CPopWindow::~CPopWindow()
{
	for(std::vector<CWnd*>::iterator iter2 = m_vtWnds.begin(); iter2 != m_vtWnds.end(); iter2++)
		delete (*iter2);
}

void CPopWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CPopWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	CreatePopControl();
	MainToPop(m_pMainWnd,this);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CPopWindow, CDialog)
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CPopWindow::Show()
{
	DoModal();
	return TRUE;
}

std::vector<SPopMain>& CPopWindow::GetPopMains()
{
	return m_vtPopMains;
}

int CPopWindow::CreatePopParamControl(SPopParamConf& paramConf)
{
	//����������̬�ı��ؼ�
	CStatic* pParamNameWnd = new CStatic();
	pParamNameWnd->Create(paramConf.m_strName.GetBuffer(),
		WS_CHILD|WS_VISIBLE,
		CRect(m_nStartX,m_nStartY,m_nStartX+paramConf.m_nWidth,m_nStartY+DEFAULT_HEIGHT),
		this,
		m_nId++);

	//���¿ؼ�����ʼ����
	m_nStartX += paramConf.m_nWidth;
	m_vtWnds.push_back(pParamNameWnd);

	CWnd* pParamWnd = NULL;
	if(paramConf.m_strCtrlType == "Edit")
	{
		CEdit* pEdit = new CEdit();
		pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			CRect(m_nStartX,m_nStartY,m_nStartX+paramConf.m_nWidth1,m_nStartY+DEFAULT_HEIGHT),
			this,
			m_nId++);

		pParamWnd = pEdit;
	}
	else if(paramConf.m_strCtrlType == "Combobox")
	{
		std::vector<SComboItemConf>& vtComboItemConf = paramConf.m_vtComboConf;
		CComboBox* pComboBox = new CComboBox();
		pComboBox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,
			CRect(m_nStartX,m_nStartY,m_nStartX+paramConf.m_nWidth1,m_nStartY+(vtComboItemConf.size()+1)*DEFAULT_HEIGHT),
			this,
			m_nId++);

		//���combobox��ѡ��
		for(size_t comboItemIdx = 0; comboItemIdx < vtComboItemConf.size(); comboItemIdx++)
			pComboBox->AddString(vtComboItemConf[comboItemIdx].m_strName.GetBuffer());

		//ѡ��Ĭ��ֵ
		for(size_t comboItemIdx = 0; comboItemIdx < vtComboItemConf.size(); comboItemIdx++)
		{
			if(vtComboItemConf[comboItemIdx].m_bChecked)
			{
				pComboBox->SetCurSel(comboItemIdx);
				break;
			}
		}

		pParamWnd = pComboBox;
	}
	else if(paramConf.m_strCtrlType == "CheckCombo")
	{
		std::vector<SComboItemConf>& vtComboItemConf = paramConf.m_vtComboConf;
		CCheckComboBox* pCheckComboBox = new CCheckComboBox();
		pCheckComboBox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,
			CRect(m_nStartX,m_nStartY,m_nStartX+paramConf.m_nWidth1,m_nStartY+(vtComboItemConf.size()+1)*DEFAULT_HEIGHT), 
			this, 
			m_nId++);
		
		//���checkcombo��ѡ��
		for(size_t comboItemIdx = 0; comboItemIdx < vtComboItemConf.size(); comboItemIdx++)
			pCheckComboBox->AddString(vtComboItemConf[comboItemIdx].m_strName.GetBuffer());

		//ѡ��Ĭ��ֵ
		for(size_t comboItemIdx = 0; comboItemIdx < vtComboItemConf.size(); comboItemIdx++)
		{
			if(vtComboItemConf[comboItemIdx].m_bChecked)
				pCheckComboBox->SetCheck(comboItemIdx, true);
		}

		pParamWnd = pCheckComboBox;
	}
	else
		return -1;

	//���¿ؼ�����ʼ����
	m_nStartX += paramConf.m_nWidth1;
	m_vtWnds.push_back(pParamWnd);

	SPopParam popParam;
	popParam.m_pPopParamConf = &paramConf;
	popParam.m_pPopParamWnd = pParamWnd;
	m_vtPopMains.back().m_vtPopParams.push_back(popParam);
	return 0;
}

int CPopWindow::CreatePopMainControl(SPopMainConf& mainConf, DWORD nMainStyle)
{
	//�������ؼ�
	CButton* pMainCtrl = new CButton();
	pMainCtrl->Create(mainConf.m_strName.GetBuffer(),
		nMainStyle,
		CRect(m_nStartX,m_nStartY,m_nStartX+mainConf.m_nWidth,m_nStartY+DEFAULT_HEIGHT),
		this,
		m_nId++);

	//����Ĭ��ֵ
	if(mainConf.m_bChecked)
		pMainCtrl->SetCheck(BST_CHECKED);

	//���¿ؼ���ʼ����
	m_nStartX += mainConf.m_nWidth;
	m_vtWnds.push_back(pMainCtrl);

	SPopMain popMain;
	popMain.m_pPopMainConf = &mainConf;
	popMain.m_pPopMainWnd = pMainCtrl;
	m_vtPopMains.push_back(popMain);

	//���������������ؼ�
	std::vector<SPopParamConf>& vtParamConf = mainConf.m_vtParams;
	for(size_t paramConfIdx = 0; paramConfIdx < vtParamConf.size(); paramConfIdx++)
		CreatePopParamControl(vtParamConf[paramConfIdx]);

	if(mainConf.m_bNewLine)
	{
		m_nStartX = 0;
		m_nStartY += DEFAULT_HEIGHT + DEFAULT_HEIGHT_INTER;
	}

	return 0;
}

int CPopWindow::CreatePopControl()
{
	//�ؼ�δ��������
	if(g_mapMainConfs.find(m_pMainWnd->GetDlgCtrlID()) == g_mapMainConfs.end())
		return -1;

	CString strConfName = g_mapMainConfs[m_pMainWnd->GetDlgCtrlID()];

	//�޸�������
	if(g_mapPopConfs.find(strConfName) == g_mapPopConfs.end())
		return -2;

	SPopConf& popConf = g_mapPopConfs[strConfName];
	CString& strConfType = popConf.m_strConfType;
	DWORD nMainStyle = WS_CHILD | WS_VISIBLE;
	if(strConfType == "Radio" || strConfType == "RadioWithArg")
		nMainStyle |= BS_AUTORADIOBUTTON;
	else if(strConfType == "Check" || strConfType == "CheckWithArg")
		nMainStyle |= BS_AUTOCHECKBOX;
	else
		return -3;

	//�������������ؼ�
	std::vector<SPopMainConf>& vtMainConf = popConf.m_vtMains;
	for(size_t mainConfIdx = 0; mainConfIdx < vtMainConf.size(); mainConfIdx++)
	{
		//��̬�����ı�
		if(vtMainConf[mainConfIdx].m_strCtrlType == "Static")
		{
			CStatic* pStatic = new CStatic();
			pStatic->Create(vtMainConf[mainConfIdx].m_strName.GetBuffer(),
				WS_CHILD|WS_VISIBLE,
				CRect(m_nStartX,m_nStartY,m_nStartX+vtMainConf[mainConfIdx].m_nWidth,m_nStartY+DEFAULT_HEIGHT),
				this,
				m_nId++);

			//������һ��
			if(vtMainConf[mainConfIdx].m_bNewLine)
			{
				m_nStartX = 0;
				m_nStartY += DEFAULT_HEIGHT + DEFAULT_HEIGHT_INTER;
			}
			m_vtWnds.push_back(pStatic);
			continue;
		}

		CreatePopMainControl(vtMainConf[mainConfIdx], nMainStyle);
	}

	return 0;
}

// CPopWindow ��Ϣ�������
afx_msg void CPopWindow::OnClose()
{
	CDialog::OnClose();
}

afx_msg void CPopWindow::OnRButtonDown(UINT nFlags, CPoint point)
{
	PopToMain(this, m_pMainWnd);
	CDialog::EndDialog(IDOK);
}

//===================================================================================

CollectionMainConfsT g_mapMainConfs;

bool REG_PROPERTY(int PID, const char* name)
{
	ASSERT(g_mapMainConfs.find(PID) == g_mapMainConfs.end());
	g_mapMainConfs.insert(std::make_pair(PID,CString(name)));
	return true;
}

//===================================================================================

CollectionPopConfsT g_mapPopConfs;

//===================================================================================

//����combobox��checkcomboѡ��
void GetComboItemConf(lua_State* L, SComboItemConf& comboItemConf)
{
	lua_getfield(L, -1, "Name");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	comboItemConf.m_strName = lua_tostring(L, -1);
	ConvertUTF8ToGBK(comboItemConf.m_strName);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Value");
	ASSERT(lua_type(L, -1) == LUA_TNUMBER);
	comboItemConf.m_nValue = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Checked");
	if(lua_type(L, -1) == LUA_TBOOLEAN)
		comboItemConf.m_bChecked = lua_toboolean(L, -1);
	lua_pop(L, 1);
}

//��������ؼ�����
void GetPopParamConf(lua_State* L, SPopParamConf& popParamConf)
{
	lua_getfield(L, -1, "CtrlType");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	popParamConf.m_strCtrlType = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Name");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	popParamConf.m_strName = lua_tostring(L, -1);
	ConvertUTF8ToGBK(popParamConf.m_strName);
	lua_pop(L, 1);

	lua_getfield(L, -1, "CName");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	popParamConf.m_strCName = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Default");
	if(lua_type(L, -1) == LUA_TSTRING)
		popParamConf.m_strDefault = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Note");
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		popParamConf.m_strNote = lua_tostring(L, -1);
		ConvertUTF8ToGBK(popParamConf.m_strNote);
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "Cast");
	if(lua_type(L, -1) == LUA_TSTRING)
		popParamConf.m_strCast = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Width");
	if(lua_type(L, -1) == LUA_TSTRING)
		popParamConf.m_nWidth = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Width1");
	if(lua_type(L, -1) == LUA_TSTRING)
		popParamConf.m_nWidth1 = lua_tointeger(L, -1);
	lua_pop(L, 1);

	if(popParamConf.m_strCtrlType == "Combobox" || popParamConf.m_strCtrlType == "CheckCombo")
	{
		lua_getfield(L, -1, "Confs");
		ASSERT(lua_type(L, -1) == LUA_TTABLE);

		//����combobox��checkcombo��ѡ��
		lua_pushnil(L);
		while(lua_next(L, -2) != 0)
		{
			ASSERT(lua_type(L, -1) == LUA_TTABLE);
			SComboItemConf comboItemConf;
			GetComboItemConf(L, comboItemConf);
			popParamConf.m_vtComboConf.push_back(comboItemConf);
			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
}

//�������ؼ�����
void GetPopMainConf(lua_State* L, SPopMainConf& popMainConf)
{
	lua_getfield(L, -1, "Name");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	popMainConf.m_strName = lua_tostring(L, -1);
	ConvertUTF8ToGBK(popMainConf.m_strName);
	lua_pop(L, 1);

	lua_getfield(L, -1, "CtrlType");
	if(lua_type(L, -1) == LUA_TSTRING)
		popMainConf.m_strCtrlType = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	if(popMainConf.m_strCtrlType != "Static")
	{
		lua_getfield(L, -1, "Value");
		ASSERT(lua_type(L, -1) == LUA_TNUMBER);
		popMainConf.m_nValue = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	lua_getfield(L, -1, "Checked");
	if(lua_type(L, -1) == LUA_TBOOLEAN)
		popMainConf.m_bChecked = lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Note");
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		popMainConf.m_strNote = lua_tostring(L, -1);
		ConvertUTF8ToGBK(popMainConf.m_strNote);
	}
	lua_pop(L, 1);

	lua_getfield(L, -1, "NewLine");
	if(lua_type(L, -1) == LUA_TBOOLEAN)
		popMainConf.m_bNewLine = lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Width");
	if(lua_type(L, -1) == LUA_TNUMBER)
		popMainConf.m_nWidth = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Params");
	if(lua_type(L, -1) == LUA_TTABLE)
	{
		//���������ؼ�
		lua_pushnil(L);
		while(lua_next(L, -2) != 0)
		{
			ASSERT(lua_type(L, -1) == LUA_TTABLE);
			SPopParamConf popParamConf;
			GetPopParamConf(L, popParamConf);
			popMainConf.m_vtParams.push_back(popParamConf);
			lua_pop(L, 1);
		}
	}
	lua_pop(L, 1);

}

//����һ������
void GetPopConf(lua_State* L, SPopConf& popConf)
{
	lua_getfield(L, -1, "ConfType");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	popConf.m_strConfType = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Confs");
	ASSERT(lua_type(L, -1) == LUA_TTABLE);

	//�������ؼ�
	lua_pushnil(L);
	while(lua_next(L, -2) != 0)
	{
		ASSERT(lua_type(L, -1) == LUA_TTABLE);
		SPopMainConf popMainConf;
		GetPopMainConf(L, popMainConf);
		popConf.m_vtMains.push_back(popMainConf);
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
}

bool LoadPopConfig(const std::string& name)
{
	//����lua����
	lua_State* L = lua_open();
	luaL_dofile(L, name.c_str());

	//��ȡ����
	lua_getglobal(L, "config");
	ASSERT(lua_type(L, -1) == LUA_TTABLE);

	//��������
	lua_pushnil(L);
	while(lua_next(L, -2) != 0)
	{
		ASSERT(lua_type(L, -1) == LUA_TTABLE);
		ASSERT(lua_type(L, -2) == LUA_TSTRING);
		CString popConfName = lua_tostring(L, -2);
		SPopConf popConf;
		GetPopConf(L, popConf);
		g_mapPopConfs.insert(std::make_pair(popConfName, popConf));
		lua_pop(L, 1);
	}

	//�ر�lua
	lua_close(L);
	return true;
}

//===================================================================================

//�������ؼ�ת���ɼ�¼
int PopParamToText(SPopParam& popParam, CString& strParamText)
{
	CString strCtrlText;
	popParam.m_pPopParamWnd->GetWindowText(strCtrlText);
	strParamText = popParam.m_pPopParamConf->m_strName + "=" + strCtrlText;

	return 0;
}

//��һ�����ã�����һ�����ؼ��Ͷ�������ؼ���ת��һ����¼
int PopMainToText(SPopMain& popMain, CString& strMainText)
{
	bool isChecked = ((CButton*)popMain.m_pPopMainWnd)->GetCheck();
	if(!isChecked)
		return -1;

	//ת�����ؼ�
	strMainText = popMain.m_pPopMainConf->m_strName;
	if(!popMain.m_vtPopParams.empty())
		strMainText += ":";

	//����ת�����в����ؼ�
	for(size_t paramIdx = 0; paramIdx < popMain.m_vtPopParams.size(); paramIdx++)
	{
		CString strParamText;
		if(PopParamToText(popMain.m_vtPopParams[paramIdx], strParamText) == 0)
			strMainText += strParamText;

		//�������һ������
		if(paramIdx < (popMain.m_vtPopParams.size() - 1))
			strMainText += ",";
	}

	return 0;
}

int PopToMain(CPopWindow* pPopWnd, CWnd* pMainWnd)
{
	ASSERT(pPopWnd && pMainWnd);
	std::vector<SPopMain>& vtPopMains = pPopWnd->GetPopMains();
	
	CHAR szClass[128];   
	GetClassName(pMainWnd->GetSafeHwnd(), szClass, 127);   
	if(lstrcmpi(szClass, "Edit")==0)
	{
		//ԭʼ�ؼ�Ϊ�༭�򣬵������ڲ�����ÿ����¼ͨ��"|"�ָ�
		CString strText;
		for(size_t mainIdx = 0; mainIdx < vtPopMains.size(); mainIdx++)
		{
			CString strMainText;
			if(PopMainToText(vtPopMains[mainIdx], strMainText) == 0)
			{
				if(strText.IsEmpty())
					strText = strMainText;
				else
					strText = strText + "|" + strMainText;
			}
		}

		((CEdit*)pMainWnd)->SetWindowText(strText.GetBuffer());
	}
	else if(lstrcmpi(szClass, "ListBox")==0)
	{
		//ԭʼ�ؼ�Ϊ�б�򣬵������ڵ�ÿ������תΪ�б���е�һ����¼
		((CListBox*)pMainWnd)->ResetContent();
		for(size_t mainIdx = 0; mainIdx < vtPopMains.size(); mainIdx++)
		{
			CString strMainText;
			if(PopMainToText(vtPopMains[mainIdx], strMainText) == 0)
				((CListBox*)pMainWnd)->AddString(strMainText.GetBuffer());
		}
	}
	else
		return -1;

	return 0;
}

//===================================================================================

//����¼ת���ɲ����ؼ��ϵ�����
int TextToPopParam(const CString& strItemText, SPopParam& popParam)
{
	//�������Ͳ���ֵͨ��=����
	int paramNamePos = strItemText.FindOneOf("=");
	if(paramNamePos != -1)
	{
		CString strParamName = strItemText.Left(paramNamePos);
		CString strParamValues = strItemText.Mid(paramNamePos + 1);

		//��������ƥ��
		if(strParamName != popParam.m_pPopParamConf->m_strName)
			return -1;

		std::vector<CString> vtParamValueTexts;
		CString strItemText;
		int nParamValuePos = 0;
		//ÿ�������ؼ�ͨ��,����
		strItemText = strParamValues.Tokenize(_T("`"),nParamValuePos);
		while (strItemText != _T(""))
		{
			vtParamValueTexts.push_back(strItemText);
			strItemText = strParamValues.Tokenize(_T("`"), nParamValuePos);
		};

		if(vtParamValueTexts.empty())
			return 0;

		if(popParam.m_pPopParamConf->m_strCtrlType == "Edit")
			((CEdit*)(popParam.m_pPopParamWnd))->SetWindowText(vtParamValueTexts[0].GetBuffer());
		else if(popParam.m_pPopParamConf->m_strCtrlType == "Combobox")
		{
			for(size_t comboIdx = 0; comboIdx < popParam.m_pPopParamConf->m_vtComboConf.size(); comboIdx++)
			{
				if(popParam.m_pPopParamConf->m_vtComboConf[comboIdx].m_strName == vtParamValueTexts[0])
				{
					((CComboBox*)(popParam.m_pPopParamWnd))->SetCurSel(comboIdx);
					break;
				}
			}
		}
		else if(popParam.m_pPopParamConf->m_strCtrlType == "CheckCombo")
		{
			for(size_t paramValueIdx = 0; paramValueIdx < vtParamValueTexts.size(); paramValueIdx++)
			{
				for(size_t comboIdx = 0; comboIdx < popParam.m_pPopParamConf->m_vtComboConf.size(); comboIdx++)
				{
					if(popParam.m_pPopParamConf->m_vtComboConf[comboIdx].m_strName == vtParamValueTexts[paramValueIdx])
					{
						((CCheckComboBox*)(popParam.m_pPopParamWnd))->SetCheck(comboIdx, true);
						break;
					}
				}
			}
		}
		else
			return -1;
	}

	return 0;
}

//�����ؼ�ȫ�����
int ClearPopParams(SPopMain& popMain)
{
	for(size_t paramIdx = 0; paramIdx < popMain.m_vtPopParams.size(); paramIdx++)
	{
		if(popMain.m_vtPopParams[paramIdx].m_pPopParamConf->m_strCtrlType == "Edit")
			((CEdit*)(popMain.m_vtPopParams[paramIdx].m_pPopParamWnd))->SetWindowText("");
		else if(popMain.m_vtPopParams[paramIdx].m_pPopParamConf->m_strCtrlType == "Combobox")
			((CComboBox*)(popMain.m_vtPopParams[paramIdx].m_pPopParamWnd))->SetCurSel(-1);
		else if(popMain.m_vtPopParams[paramIdx].m_pPopParamConf->m_strCtrlType == "CheckCombo")
			((CCheckComboBox*)(popMain.m_vtPopParams[paramIdx].m_pPopParamWnd))->SelectAll(false);
	}

	return 0;
}

//��һ����¼ת��һ�����ã�����һ�����ؼ��Ͷ�������ؼ���
int TextToPopMain(const CString& strItemText, CPopWindow* pPopWnd)
{
	ASSERT(pPopWnd);
	std::vector<SPopMain>& vtPopMains = pPopWnd->GetPopMains();

	CString strMainText;
	CString strParamsText;

	//���ؼ��Ͳ����ؼ�ͨ��:����
	int nMainPos = strItemText.FindOneOf(":");
	if(nMainPos != -1)
	{
		strMainText = strItemText.Left(nMainPos);
		strParamsText = strItemText.Mid(nMainPos + 1);
	}
	else
		strMainText = strItemText;

	//����ƥ������ؼ�
	for(size_t mainIdx = 0; mainIdx < vtPopMains.size(); mainIdx++)
	{
		if(vtPopMains[mainIdx].m_pPopMainConf->m_strName == strMainText)
		{
			std::vector<CString> vtParamTexts;
			CString strItemText;
			int nParamPos = 0;
			//ÿ�������ؼ�ͨ��,����
			strItemText = strParamsText.Tokenize(_T(","),nParamPos);
			while (strItemText != _T(""))
			{
				vtParamTexts.push_back(strItemText);
				strItemText = strParamsText.Tokenize(_T(","), nParamPos);
			};

			//�����ؼ�������ƥ��
			if(vtParamTexts.size() != vtPopMains[mainIdx].m_vtPopParams.size())
				return -1;

			//���ؼ�ѡ��
			((CButton*)(vtPopMains[mainIdx].m_pPopMainWnd))->SetCheck(1);

			//�����ؼ�ȫ�����
			ClearPopParams(vtPopMains[mainIdx]);

			//���ò����ؼ���ֵ
			for(size_t paramIdx = 0; paramIdx < vtPopMains[mainIdx].m_vtPopParams.size(); paramIdx++)
				TextToPopParam(vtParamTexts[paramIdx], vtPopMains[mainIdx].m_vtPopParams[paramIdx]);

			break;
		}
	}

	return 0;
}

//�����ؼ�ȫ����Ϊδѡ��
int ClearPopMains(CPopWindow* pPopWnd)
{
	ASSERT(pPopWnd);
	std::vector<SPopMain>& vtPopMains = pPopWnd->GetPopMains();

	for(size_t mainIdx = 0; mainIdx < vtPopMains.size(); mainIdx++)
		((CButton*)(vtPopMains[mainIdx].m_pPopMainWnd))->SetCheck(0);

	return 0;
}

//�����ؼ��ϵ�����ת��һ���е��������ı�
int MainToMainTexts(CWnd* pMainWnd, std::vector<CString>& vtMainTexts)
{
	CHAR szClass[128];
	GetClassName(pMainWnd->GetSafeHwnd(), szClass, 127);   
	if(lstrcmpi(szClass, "Edit")==0)
	{
		CString strText;
		((CEdit*)pMainWnd)->GetWindowText(strText);

		CString strItemText;
		int nMainPos = 0;
		//ÿһ�пؼ�ͨ��|����
		strItemText = strText.Tokenize(_T("|"),nMainPos);
		while (strItemText != _T(""))
		{
			vtMainTexts.push_back(strItemText);
			strItemText = strText.Tokenize(_T("|"), nMainPos);
		};
	}
	else if(lstrcmpi(szClass, "ListBox")==0)
	{
		//�������ؼ���ֵ
		CString strItemText;
		for(int idx = 0; idx < ((CListBox*)pMainWnd)->GetCount(); idx++)
		{
			((CListBox*)pMainWnd)->GetText(idx,strItemText);
			vtMainTexts.push_back(strItemText);
		}
	}
	else
		return -1;

	return 0;
}

int MainToPop(CWnd* pMainWnd, CPopWindow* pPopWnd)
{
	ASSERT(pPopWnd && pMainWnd);
	std::vector<CString> vtMainTexts;
	if(MainToMainTexts(pMainWnd, vtMainTexts) != 0)
		return -1;
	
	//Ϊ�գ�ʹ�õ������õ�Ĭ��ֵ
	if(vtMainTexts.empty())
		return 0;

	//��������ÿؼ���Ĭ��ֵ
	ClearPopMains(pPopWnd);

	//��ÿ��ֵд���������ؼ���
	for(size_t idx = 0; idx < vtMainTexts.size(); idx++)
		TextToPopMain(vtMainTexts[idx], pPopWnd);

	return 0;
}

//===================================================================================

//�ڲ��洢�Ĳ�����ʽ
enum EParamDataType
{
	PARAM_DATA_TYPE_UNKNOWN,
	PARAM_DATA_TYPE_INT,		//int��
	PARAM_DATA_TYPE_FLOAT,		//float��
	PARAM_DATA_TYPE_STRING,		//string��
	PARAM_DATA_TYPE_BOOL,		//bool��
	PARAM_DATA_TYPE_ARRAY_INT,	//int������
};

//�ڲ��洢�Ĳ�������
struct SPopParamData
{
	CString m_strCName;			//������
	int m_nDataType;			//��������

	//��������union������ֵ
	int m_nValue;
	float m_fValue;
	CString m_strValue;
	bool m_bValue;
	std::vector<int> m_vtValue;

	SPopParamData()
	{
		m_strCName = "";
		m_nDataType = PARAM_DATA_TYPE_UNKNOWN;
		m_nValue = -1;
		m_fValue = 0.0;
		m_strValue = "";
		m_bValue = false;
		m_vtValue.clear();
	}
};

//�ڲ��洢��������
struct SPopMainData
{
	int m_nValue;							//���ؼ�ֵ
	std::vector<SPopParamData> m_vtParams;	//����ֵ
	SPopMainData()
	{
		m_nValue = -1;
		m_vtParams.clear();
	}
};

//��ȡĬ�ϲ���ֵ
//ע�������õ�atoi��atof��������������̫��ȫ�����Ժ�����
int GetDefaultParamDataAndText(SPopParamConf& popParamConf, SPopParamData& popParamData, CString& strParamText)
{
	popParamData.m_strCName = popParamConf.m_strCName;
	strParamText = popParamConf.m_strName + "=";
	if(popParamConf.m_strCtrlType == "Edit")
	{
		if(popParamConf.m_strCast == "Integer")
		{
			popParamData.m_nDataType = PARAM_DATA_TYPE_INT;
			popParamData.m_nValue = atoi(popParamConf.m_strDefault.GetBuffer());
		}
		else if(popParamConf.m_strCast == "Float")
		{
			popParamData.m_nDataType = PARAM_DATA_TYPE_FLOAT;
			popParamData.m_fValue = atof(popParamConf.m_strDefault.GetBuffer());
		}
		else if(popParamConf.m_strCast == "Bool")
		{
			popParamData.m_nDataType = PARAM_DATA_TYPE_BOOL;
			if(popParamConf.m_strDefault.IsEmpty() || popParamConf.m_strDefault == "0")
				popParamData.m_bValue = false;
			else
				popParamData.m_bValue = true;
		}
		else
		{
			popParamData.m_nDataType = PARAM_DATA_TYPE_STRING;
			popParamData.m_strValue = popParamConf.m_strDefault;
		}
		strParamText += popParamConf.m_strDefault;
	}
	else if(popParamConf.m_strCtrlType == "Combobox")
	{
		popParamData.m_nDataType = PARAM_DATA_TYPE_INT;
		for(size_t comboIdx = 0; comboIdx < popParamConf.m_vtComboConf.size(); comboIdx++)
		{
			if(popParamConf.m_vtComboConf[comboIdx].m_bChecked)
			{
				popParamData.m_nValue = popParamConf.m_vtComboConf[comboIdx].m_nValue;
				strParamText += popParamConf.m_vtComboConf[comboIdx].m_strName;
				break;
			}
		}
	}
	else if(popParamConf.m_strCtrlType == "CheckCombo")
	{
		popParamData.m_nDataType = PARAM_DATA_TYPE_ARRAY_INT;
		for(size_t comboIdx = 0; comboIdx < popParamConf.m_vtComboConf.size(); comboIdx++)
		{
			if(popParamConf.m_vtComboConf[comboIdx].m_bChecked)
			{
				popParamData.m_vtValue.push_back(popParamConf.m_vtComboConf[comboIdx].m_nValue);
				if(comboIdx)
					strParamText += "`";

				strParamText += popParamConf.m_vtComboConf[comboIdx].m_strName;
			}
		}
	}
	else
		return -1;

	return 0;
}

//��ȡĬ����ֵ
int GetDefaultMainDataAndText(SPopMainConf& popMainConf, SPopMainData& popMainData, CString& strMainText)
{
	//���ؼ�δѡ��
	if(!popMainConf.m_bChecked)
		return -1;

	//�����ؼ���ֵ
	popMainData.m_nValue = popMainConf.m_nValue;
	strMainText = popMainConf.m_strName;

	if(!popMainConf.m_vtParams.empty())
		strMainText += ":";

	//���δ�����ؼ���ֵ
	for(size_t paramIdx = 0; paramIdx < popMainConf.m_vtParams.size(); paramIdx++)
	{
		SPopParamData popParamData;
		CString strParamText = "";
		if(GetDefaultParamDataAndText(popMainConf.m_vtParams[paramIdx], popParamData, strParamText) == 0)
		{
			popMainData.m_vtParams.push_back(popParamData);
			if(paramIdx)
				strMainText += ",";

			strMainText += strParamText;
		}
		else
			return -2;
	}

	return 0;
}

//��ȡ����Ĭ��ֵ
int GetDefaultDataAndText(SPopConf& popConf, std::vector<SPopMainData>& vtPopMainData, std::vector<CString>& vtMainText)
{
	std::vector<SPopMainConf>& vtPopMainConf = popConf.m_vtMains;
	for(size_t mainIdx = 0; mainIdx < vtPopMainConf.size(); mainIdx++)
	{
		SPopMainData popMainData;
		CString strMainText = "";
		if(GetDefaultMainDataAndText(vtPopMainConf[mainIdx], popMainData, strMainText) == 0)
		{
			vtPopMainData.push_back(popMainData);
			vtMainText.push_back(strMainText);
		}
	}

	return 0;
}

//�����ı�ת���ڲ��洢��ʽ
int ParamTextToData(CString& strItemText, SPopParamConf& popParamConf, SPopParamData& popParamData)
{
	//�������Ͳ���ֵͨ��=����
	int paramNamePos = strItemText.FindOneOf("=");
	if(paramNamePos != -1)
	{
		CString strParamName = strItemText.Left(paramNamePos);
		CString strParamValues = strItemText.Mid(paramNamePos + 1);

		//��������ƥ��
		if(strParamName != popParamConf.m_strName)
			return -1;

		popParamData.m_strCName = popParamConf.m_strCName;

		std::vector<CString> vtParamValueTexts;
		CString strItemText;
		int nParamValuePos = 0;
		//ÿ�������ؼ�ͨ��,����
		strItemText = strParamValues.Tokenize(_T("`"),nParamValuePos);
		while (strItemText != _T(""))
		{
			vtParamValueTexts.push_back(strItemText);
			strItemText = strParamValues.Tokenize(_T("`"), nParamValuePos);
		};

		//������ֵ��ʹ��Ĭ��ֵ
		//Edit��Combobox������Ĭ��ֵ����CheckCombo��Ĭ��ֵʼ��Ϊ��
		if(vtParamValueTexts.empty() && (popParamConf.m_strCtrlType == "Edit" || popParamConf.m_strCtrlType == "Combobox"))
		{
			CString strUnused;
			GetDefaultParamDataAndText(popParamConf, popParamData, strUnused);
			return 0;
		}

		if(popParamConf.m_strCtrlType == "Edit")
		{
			if(popParamConf.m_strCast == "Integer")
			{
				popParamData.m_nDataType = PARAM_DATA_TYPE_INT;
				popParamData.m_nValue = atoi(vtParamValueTexts[0].GetBuffer());
			}
			else if(popParamConf.m_strCast == "Float")
			{
				popParamData.m_nDataType = PARAM_DATA_TYPE_FLOAT;
				popParamData.m_fValue = atof(vtParamValueTexts[0].GetBuffer());
			}
			else if(popParamConf.m_strCast == "Bool")
			{
				popParamData.m_nDataType = PARAM_DATA_TYPE_BOOL;
				if(vtParamValueTexts[0].IsEmpty() || vtParamValueTexts[0] == "")
					popParamData.m_bValue = false;
				else
					popParamData.m_bValue = true;
			}
			else
			{
				popParamData.m_nDataType = PARAM_DATA_TYPE_STRING;
				popParamData.m_strValue = vtParamValueTexts[0];
			}
		}
		else if(popParamConf.m_strCtrlType == "Combobox")
		{
			popParamData.m_nDataType = PARAM_DATA_TYPE_INT;
			for(size_t comboIdx = 0; comboIdx < popParamConf.m_vtComboConf.size(); comboIdx++)
			{
				if(popParamConf.m_vtComboConf[comboIdx].m_strName == vtParamValueTexts[0])
				{
					popParamData.m_nValue = popParamConf.m_vtComboConf[comboIdx].m_nValue;
					break;
				}
			}
		}
		else if(popParamConf.m_strCtrlType == "CheckCombo")
		{
			popParamData.m_nDataType = PARAM_DATA_TYPE_ARRAY_INT;
			for(size_t paramValueIdx = 0; paramValueIdx < vtParamValueTexts.size(); paramValueIdx++)
			{
				for(size_t comboIdx = 0; comboIdx < popParamConf.m_vtComboConf.size(); comboIdx++)
				{
					if(popParamConf.m_vtComboConf[comboIdx].m_strName == vtParamValueTexts[paramValueIdx])
					{
						popParamData.m_vtValue.push_back(popParamConf.m_vtComboConf[comboIdx].m_nValue);
						break;
					}
				}
			}
		}
		else
			return -1;
	}

	return 0;
}

//�������ı�ת���ڲ��洢��ʽ
int MainTextToData(CString& strItemText, std::vector<SPopMainConf>& vtPopMainConf, SPopMainData& popMainData)
{
	CString strMainText;
	CString strParamsText;

	//���ؼ��Ͳ����ؼ�ͨ��:����
	int nMainPos = strItemText.FindOneOf(":");
	if(nMainPos != -1)
	{
		strMainText = strItemText.Left(nMainPos);
		strParamsText = strItemText.Mid(nMainPos + 1);
	}
	else
		strMainText = strItemText;

	//����ƥ������ؼ�
	for(size_t mainIdx = 0; mainIdx < vtPopMainConf.size(); mainIdx++)
	{
		if(vtPopMainConf[mainIdx].m_strName == strMainText)
		{
			std::vector<CString> vtParamTexts;
			CString strItemText;
			int nParamPos = 0;
			//ÿ�������ؼ�ͨ��,����
			strItemText = strParamsText.Tokenize(_T(","),nParamPos);
			while (strItemText != _T(""))
			{
				vtParamTexts.push_back(strItemText);
				strItemText = strParamsText.Tokenize(_T(","), nParamPos);
			};

			//�����ؼ�������ƥ��
			if(vtParamTexts.size() != vtPopMainConf[mainIdx].m_vtParams.size())
				return -1;

			//���ؼ�ѡ��
			popMainData.m_nValue = vtPopMainConf[mainIdx].m_nValue;

			//���ò����ؼ���ֵ
			for(size_t paramIdx = 0; paramIdx < vtPopMainConf[mainIdx].m_vtParams.size(); paramIdx++)
			{
				SPopParamData popParamData;
				ParamTextToData(vtParamTexts[paramIdx], vtPopMainConf[mainIdx].m_vtParams[paramIdx], popParamData);
				popMainData.m_vtParams.push_back(popParamData);
			}

			break;
		}
	}

	return 0;
}

//�ı�ת���ڲ��洢��ʽ
int TextToData(SPopConf& popConf, std::vector<CString>& vtMainTexts, std::vector<SPopMainData>& vtPopMainData)
{
	for(size_t mainIdx = 0; mainIdx < vtMainTexts.size(); mainIdx++)
	{
		SPopMainData popMainData;
		if(MainTextToData(vtMainTexts[mainIdx], popConf.m_vtMains, popMainData) == 0)
			vtPopMainData.push_back(popMainData);
	}
	return 0;
}

int RadioToDB(std::vector<SPopMainData> vtPopMainData, lua_State* L, const std::string& strName)
{
	ASSERT(lua_istable(L, -1));
	if(vtPopMainData.empty())
		lua_pushinteger(L, -1);
	else
		lua_pushinteger(L, vtPopMainData[0].m_nValue);

	lua_setfield(L, -2, strName.c_str());
	return 0;
}

int RadioWithArgToDB(std::vector<SPopMainData> vtPopMainData, lua_State* L, const std::string& strName)
{
	ASSERT(lua_istable(L, -1));
	lua_newtable(L);
	if(!vtPopMainData.empty())
	{
		//�洢��ֵ
		lua_pushinteger(L, 1);
		lua_pushinteger(L, vtPopMainData[0].m_nValue);
		lua_settable(L, -3);

		//���δ洢���в���ֵ
		for(size_t paramIdx = 0; paramIdx < vtPopMainData[0].m_vtParams.size(); paramIdx++)
		{
			switch(vtPopMainData[0].m_vtParams[paramIdx].m_nDataType)
			{
			case PARAM_DATA_TYPE_INT:
				lua_pushinteger(L, vtPopMainData[0].m_vtParams[paramIdx].m_nValue);
				break;
			case PARAM_DATA_TYPE_FLOAT:
				lua_pushnumber(L, vtPopMainData[0].m_vtParams[paramIdx].m_fValue);
				break;
			case PARAM_DATA_TYPE_BOOL:
				lua_pushboolean(L, vtPopMainData[0].m_vtParams[paramIdx].m_bValue);
				break;
			case PARAM_DATA_TYPE_STRING:
				lua_pushstring(L, vtPopMainData[0].m_vtParams[paramIdx].m_strValue.GetBuffer());
				break;
			case PARAM_DATA_TYPE_ARRAY_INT:
				{
					std::vector<int>& vtValues = vtPopMainData[0].m_vtParams[paramIdx].m_vtValue;
					lua_newtable(L);
					for(size_t valueIdx = 0; valueIdx < vtValues.size(); valueIdx++)
					{
						lua_pushinteger(L, valueIdx);
						lua_pushinteger(L, vtValues[valueIdx]);
						lua_settable(L, -3);
					}
				}
				break;
			default:
				ASSERT(false && "Unknown type");
			}
			lua_setfield(L, -2, vtPopMainData[0].m_vtParams[paramIdx].m_strCName.GetBuffer());
		}
	}

	lua_setfield(L, -2, strName.c_str());
	return 0;
}

int CheckToDB(std::vector<SPopMainData> vtPopMainData, lua_State* L, const std::string& strName)
{
	ASSERT(lua_istable(L, -1));
	lua_newtable(L);
	for(size_t mainIdx = 0; mainIdx < vtPopMainData.size(); mainIdx++)
	{
		lua_pushinteger(L, mainIdx);
		lua_pushinteger(L, vtPopMainData[mainIdx].m_nValue);
		lua_settable(L, -3);
	}

	lua_setfield(L, -2, strName.c_str());
	return 0;
}

int CheckWithArgToDB(std::vector<SPopMainData> vtPopMainData, lua_State* L, const std::string& strName)
{
	ASSERT(lua_istable(L, -1));
	lua_newtable(L);

	for(size_t mainIdx = 0; mainIdx < vtPopMainData.size(); mainIdx++)
	{
		lua_pushinteger(L, vtPopMainData[mainIdx].m_nValue);
		lua_newtable(L);
		//���δ洢���в���ֵ
		std::vector<SPopParamData>& vtPopParamData = vtPopMainData[mainIdx].m_vtParams;
		for(size_t paramIdx = 0; paramIdx < vtPopParamData.size(); paramIdx++)
		{
			switch(vtPopParamData[paramIdx].m_nDataType)
			{
			case PARAM_DATA_TYPE_INT:
				lua_pushinteger(L, vtPopParamData[paramIdx].m_nValue);
				break;
			case PARAM_DATA_TYPE_FLOAT:
				lua_pushnumber(L, vtPopParamData[paramIdx].m_fValue);
				break;
			case PARAM_DATA_TYPE_BOOL:
				lua_pushboolean(L, vtPopParamData[paramIdx].m_bValue);
				break;
			case PARAM_DATA_TYPE_STRING:
				lua_pushstring(L, vtPopParamData[paramIdx].m_strValue.GetBuffer());
				break;
			case PARAM_DATA_TYPE_ARRAY_INT:
				{
					std::vector<int>& vtValues = vtPopParamData[paramIdx].m_vtValue;
					lua_newtable(L);
					for(size_t valueIdx = 0; valueIdx < vtValues.size(); valueIdx++)
					{
						lua_pushinteger(L, valueIdx);
						lua_pushinteger(L, vtValues[valueIdx]);
						lua_settable(L, -3);
					}
				}
				break;
			default:
				ASSERT(false && "Unknown type");
			}
			lua_setfield(L, -2, vtPopParamData[paramIdx].m_strCName.GetBuffer());
		}
		lua_settable(L, -3);
	}

	lua_setfield(L, -2, strName.c_str());
	return 0;
}

int MainToDB(CWnd* pMainWnd, lua_State* L, const std::string& strName)
{
	ASSERT(pMainWnd && L);
	if(g_mapMainConfs.find(pMainWnd->GetDlgCtrlID()) == g_mapMainConfs.end())
		return -1;

	CString strXmlName = g_mapMainConfs[pMainWnd->GetDlgCtrlID()];
	if(g_mapPopConfs.find(strXmlName) == g_mapPopConfs.end())
		return -2;

	SPopConf& popConf = g_mapPopConfs[strXmlName];

	std::vector<CString> vtMainTexts;
	if(MainToMainTexts(pMainWnd, vtMainTexts) != 0)
		return -3;

	std::vector<SPopMainData> vtPopMainData;
	std::vector<CString> vtUnused;
	//Radio������Ĭ��ֵ����chech��Ĭ��ֵʼ��Ϊ��
	if(vtMainTexts.empty() && (popConf.m_strConfType == "Radio" || popConf.m_strConfType == "RadioWithArg"))
		GetDefaultDataAndText(popConf, vtPopMainData, vtUnused);
	else
		TextToData(popConf, vtMainTexts, vtPopMainData);

	CString strConfType = popConf.m_strConfType;
	if(strConfType == "Radio")
		RadioToDB(vtPopMainData, L, strName);
	else if(strConfType == "RadioWithArg")
		RadioWithArgToDB(vtPopMainData, L, strName);
	else if(strConfType == "Check")
		CheckToDB(vtPopMainData, L, strName);
	else if(strConfType == "CheckWithArg")
		CheckWithArgToDB(vtPopMainData, L, strName);
	else
		return -4;

	return 0;
}

//===================================================================================

int DBToRadio(lua_State* L, const std::string& strName, std::vector<SPopMainData>& vtPopMainData)
{
	ASSERT(lua_istable(L, -1));

	//��ȡ��ֵ
	lua_getfield(L, -1, strName.c_str());
	if(lua_type(L, -1) != LUA_TNUMBER)
	{
		lua_pop(L, 1);
		return -1;
	}
	int nMainValue = lua_tointeger(L, -1);
	lua_pop(L, 1);

	SPopMainData popMainData;
	popMainData.m_nValue = nMainValue;
	vtPopMainData.push_back(popMainData);
	return 0;
}

int DBToRadioWithArg(lua_State* L, const std::string& strName, std::vector<SPopMainData>& vtPopMainData)
{
	ASSERT(lua_istable(L, -1));

	int nTopIndex = lua_gettop(L);

	lua_getfield(L, -1, strName.c_str());
	//�������ò����ڻ��޷�����
	if(lua_type(L, -1) != LUA_TTABLE)
	{
		lua_settop(L, nTopIndex);
		return -1;
	}

	SPopMainData popMainData;

	//ȡ��ֵ
	lua_pushinteger(L, 1);
	lua_gettable(L, -2);
	if(lua_type(L, -1) != LUA_TNUMBER)
	{
		lua_settop(L, nTopIndex);
		return -2;
	}
	popMainData.m_nValue = lua_tointeger(L, -1);
	lua_pop(L, 1);

	//����ȡ����
	lua_pushnil(L);
	while(lua_next(L, -2) != 0)
	{
		switch(lua_type(L, -2))
		{
		case LUA_TNUMBER:	//��ֵ
			break;
		case LUA_TSTRING:	//����
			{
				SPopParamData popParamData;
				popParamData.m_strCName = lua_tostring(L, -2);
				switch(lua_type(L, -1))
				{
				case LUA_TNUMBER:
					popParamData.m_nDataType = PARAM_DATA_TYPE_INT;
					popParamData.m_nValue = lua_tointeger(L, -1);
					popParamData.m_fValue = lua_tonumber(L, -1);
					break;
				case LUA_TSTRING:
					popParamData.m_nDataType = PARAM_DATA_TYPE_STRING;
					popParamData.m_strValue = lua_tostring(L, -1);
					break;
				case LUA_TBOOLEAN:
					popParamData.m_nDataType = PARAM_DATA_TYPE_BOOL;
					popParamData.m_bValue = lua_toboolean(L, -1);
					break;
				case LUA_TTABLE:
					popParamData.m_nDataType = PARAM_DATA_TYPE_ARRAY_INT;
					lua_pushnil(L);
					while(lua_next(L, -2) != 0)
					{
						if(lua_type(L, -1) != LUA_TNUMBER || lua_type(L, -2) != LUA_TNUMBER)
						{
							lua_settop(L, nTopIndex);
							return -4;
						}
						popParamData.m_vtValue.push_back(lua_tointeger(L, -1));
						lua_pop(L, 1);
					}
					break;
				default:
					lua_settop(L, nTopIndex);
					return -5;
				}
				popMainData.m_vtParams.push_back(popParamData);
			}
			break;
		default:
			lua_settop(L, nTopIndex);
			return -6;
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
	vtPopMainData.push_back(popMainData);
	return 0;
}

int DBToCheck(lua_State* L, const std::string& strName, std::vector<SPopMainData>& vtPopMainData)
{
	ASSERT(lua_istable(L, -1));
	int nTopIndex = lua_gettop(L);
	lua_getfield(L, -1, strName.c_str());
	//�������ò����ڻ��޷�����
	if(lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pop(L, 1);
		return -1;
	}

	lua_pushnil(L);
	while(lua_next(L, -2) != 0)
	{
		//��ֵ����������ֵ���붼Ϊint
		if(lua_type(L, -2) != LUA_TNUMBER || lua_type(L, -1) != LUA_TNUMBER)
		{
			lua_settop(L, nTopIndex);
			return -2;
		}
		
		SPopMainData popMainData;
		popMainData.m_nValue = lua_tointeger(L, -1);
		vtPopMainData.push_back(popMainData);
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
	return 0;
}

int DBToCheckWithArg(lua_State* L, const std::string& strName, std::vector<SPopMainData>& vtPopMainData)
{
	ASSERT(lua_istable(L, -1));
	int nTopIndex = lua_gettop(L);
	lua_getfield(L, -1, strName.c_str());
	//�������ò����ڻ��޷�����
	if(lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pop(L, 1);
		return -1;
	}

	lua_pushnil(L);
	while(lua_next(L, -2) != 0)
	{
		//��ֵ����Ϊint�����в�����һ��table��
		if(lua_type(L, -2) != LUA_TNUMBER || lua_type(L, -1) != LUA_TTABLE)
		{
			lua_settop(L, nTopIndex);
			return -2;
		}

		SPopMainData popMainData;
		popMainData.m_nValue = lua_tointeger(L, -2);

		lua_pushnil(L);
		while(lua_next(L, -2) != 0)
		{
			SPopParamData popParamData;
			if(lua_type(L, -2) != LUA_TSTRING)
			{
				lua_settop(L, nTopIndex);
				return -3;
			}
			popParamData.m_strCName = lua_tostring(L, -2);

			switch(lua_type(L, -1))
			{
			case LUA_TNUMBER:
				popParamData.m_nDataType = PARAM_DATA_TYPE_INT;
				popParamData.m_nValue = lua_tointeger(L, -1);
				popParamData.m_fValue = lua_tonumber(L, -1);
				break;
			case LUA_TSTRING:
				popParamData.m_nDataType = PARAM_DATA_TYPE_STRING;
				popParamData.m_strValue = lua_tostring(L, -1);
				break;
			case LUA_TBOOLEAN:
				popParamData.m_nDataType = PARAM_DATA_TYPE_BOOL;
				popParamData.m_bValue = lua_toboolean(L, -1);
				break;
			case LUA_TTABLE:
				popParamData.m_nDataType = PARAM_DATA_TYPE_ARRAY_INT;
				{
					lua_pushnil(L);
					while(lua_next(L, -2) != 0)
					{
						if(lua_type(L, -1) != LUA_TNUMBER || lua_type(L, -2) != LUA_TNUMBER)
						{
							lua_settop(L, nTopIndex);
							return -4;
						}
						popParamData.m_vtValue.push_back(lua_tointeger(L, -1));
						lua_pop(L, 1);
					}
				}
				break;
			}
			popMainData.m_vtParams.push_back(popParamData);
			lua_pop(L, 1);
		}

		vtPopMainData.push_back(popMainData);
		lua_pop(L, 1);
	}

	lua_pop(L, 1);
	return 0;
}

int DataToText(SPopConf& popConf, std::vector<SPopMainData>& vtPopMainData, std::vector<CString>& vtMainTexts)
{
	return 0;
}

int MainTextsToMain(std::vector<CString>& vtMainTexts, CWnd* pMainWnd)
{
	return 0;
}

int DBToMain(lua_State* L, const std::string& strName, CWnd* pMainWnd)
{
	ASSERT(pMainWnd && L);
	if(g_mapMainConfs.find(pMainWnd->GetDlgCtrlID()) == g_mapMainConfs.end())
		return -1;

	CString strXmlName = g_mapMainConfs[pMainWnd->GetDlgCtrlID()];
	if(g_mapPopConfs.find(strXmlName) == g_mapPopConfs.end())
		return -2;

	SPopConf& popConf = g_mapPopConfs[strXmlName];

	//����DB�е�����
	std::vector<SPopMainData> vtPopMainData;
	CString strConfType = popConf.m_strConfType;
	if(strConfType == "Radio")
		DBToRadio(L, strName, vtPopMainData);
	else if(strConfType == "RadioWithArg")
		DBToRadioWithArg(L, strName, vtPopMainData);
	else if(strConfType == "Check")
		DBToCheck(L, strName, vtPopMainData);
	else if(strConfType == "CheckWithArg")
		DBToCheckWithArg(L, strName, vtPopMainData);
	else
		return -3;

	std::vector<SPopMainData> vtUnused;
	std::vector<CString> vtMainTexts;
	//Radio������Ĭ��ֵ����chech��Ĭ��ֵʼ��Ϊ��
	if(vtPopMainData.empty() && (popConf.m_strConfType == "Radio" || popConf.m_strConfType == "RadioWithArg"))
		GetDefaultDataAndText(popConf, vtUnused, vtMainTexts);
	else
		DataToText(popConf, vtPopMainData, vtMainTexts);

	if(MainTextsToMain(vtMainTexts, pMainWnd) != 0)
		return -4;

	return 0;
}

//===================================================================================

void ConvertGBKToUTF8(CString& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
	unsigned short* wszUTF8 = new unsigned short[len + 1];
	memset(wszUTF8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUTF8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUTF8, -1, NULL, 0, NULL, NULL);
	char* szUTF8 = new char[len + 1];
	memset(szUTF8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUTF8, -1, szUTF8, len, NULL, NULL);

	strGBK = szUTF8;
	delete[] szUTF8;
	delete[] wszUTF8;
}

void ConvertUTF8ToGBK(CString& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8, -1, NULL, 0);
	unsigned short* wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	strUTF8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}