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

void CPopWindow::SetMain(CWnd* pWnd)
{
	m_pMainWnd = pWnd;
}

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
int TextToPopParam(CString& strItemText, SPopParam& popParam)
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
int TextToPopMain(CString& strItemText, CPopWindow* pPopWnd)
{
	ASSERT(pPopWnd);
	std::vector<SPopMain>& vtPopMains = pPopWnd->GetPopMains();

	//���ؼ��Ͳ����ؼ�ͨ��:����
	int nMainPos = strItemText.FindOneOf(":");
	if(nMainPos != -1)
	{
		CString strMainText = strItemText.Left(nMainPos);
		CString strParamsText = strItemText.Mid(nMainPos + 1);

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

int MainToPop(CWnd* pMainWnd, CPopWindow* pPopWnd)
{
	ASSERT(pPopWnd && pMainWnd);
	CHAR szClass[128];   
	GetClassName(pMainWnd->GetSafeHwnd(), szClass, 127);   
	if(lstrcmpi(szClass, "Edit")==0)
	{
		CString strText;
		((CEdit*)pMainWnd)->GetWindowText(strText);
		if(strText.IsEmpty())
			return -1;

		//��������ÿؼ���Ĭ��ֵ
		ClearPopMains(pPopWnd);

		CString strItemText;
		int nMainPos = 0;
		//ÿһ�пؼ�ͨ��|����
		strItemText = strText.Tokenize(_T("|"),nMainPos);
		while (strItemText != _T(""))
		{
			TextToPopMain(strItemText, pPopWnd);
			strItemText = strText.Tokenize(_T("|"), nMainPos);
		};
	}
	else if(lstrcmpi(szClass, "ListBox")==0)
	{
		if(((CListBox*)pMainWnd)->GetCount() <= 0)
			return -1;

		//��������ÿؼ���Ĭ��ֵ
		ClearPopMains(pPopWnd);

		//�������ؼ���ֵ
		CString strItemText;
		for(int idx = 0; idx < ((CListBox*)pMainWnd)->GetCount(); idx++)
		{
			((CListBox*)pMainWnd)->GetText(idx,strItemText);
			TextToPopMain(strItemText, pPopWnd);
		}
	}
	else
		return -1;

	return 0;
}

//===================================================================================

int MainToDB(CWnd* pMainWnd, lua_State* L, std::string& strName)
{
	return 0;
}

//===================================================================================

int DBToMain(lua_State* L, std::string& strName, CWnd* pMainWnd)
{
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