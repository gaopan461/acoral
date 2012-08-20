// PopWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTest.h"
#include "PopWindow.h"
#include "CheckComboBox.h"

//===================================================================================
bool g_bIsPopMenu = true;

//===================================================================================
// CPopWindow 对话框

IMPLEMENT_DYNAMIC(CPopWindow, CDialog)

CPopWindow::CPopWindow(int resID, CWnd* pParent /*=NULL*/)
	: CDialog(resID, pParent)
	, m_nId(USER_CTRL_ID_START)
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
	CreateConfigControls();
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

int CPopWindow::CreateConfigControls()
{
	//控件未关联配置
	if(g_mapMainConfs.find(m_pMainWnd->GetDlgCtrlID()) == g_mapMainConfs.end())
		return -1;

	CString strConfName = g_mapMainConfs[m_pMainWnd->GetDlgCtrlID()];

	//无该项配置
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
	
	//nStartX和nStartY分别记录当前控件的起始坐标
	int nStartX = 0;
	int nStartY = 0;

	//遍历并创建主控件
	for(std::vector<SPopMainConf>::iterator mainConfItem = popConf.m_vtMains.begin(); 
		mainConfItem != popConf.m_vtMains.end(); 
		mainConfItem++)
	{
		//静态描述文本
		if(mainConfItem->m_strCtrlType == "Static")
		{
			//换到下一行
			nStartX = 0;
			nStartY += DEFAULT_HEIGHT;

			CStatic* pStatic = new CStatic();
			pStatic->Create(mainConfItem->m_strName.GetBuffer(),WS_CHILD|WS_VISIBLE,CRect(nStartX,nStartY,nStartX+mainConfItem->m_nWidth,nStartY+DEFAULT_HEIGHT),this,m_nId++);

			//换到下一行
			nStartY += DEFAULT_HEIGHT;
			m_vtWnds.push_back(pStatic);
			continue;
		}

		//创建主控件
		CButton* pMainCtrl = new CButton();
		pMainCtrl->Create(mainConfItem->m_strName.GetBuffer(),nMainStyle,CRect(nStartX,nStartY,nStartX+mainConfItem->m_nWidth,nStartY+DEFAULT_HEIGHT),this,m_nId++);

		//更新控件起始坐标
		nStartX += mainConfItem->m_nWidth;
		m_vtWnds.push_back(pMainCtrl);

		SPopMain popMain;
		popMain.m_pPopMainConf = &(*mainConfItem);
		popMain.m_pPopMainWnd = pMainCtrl;

		//遍历并创建参数控件
		for(std::vector<SPopParamConf>::iterator paramConfItem = mainConfItem->m_vtParams.begin();
			paramConfItem != mainConfItem->m_vtParams.end();
			paramConfItem++)
		{
			//参数名，静态文本控件
			CStatic* pParamNameWnd = new CStatic();
			pParamNameWnd->Create(paramConfItem->m_strName.GetBuffer(),WS_CHILD|WS_VISIBLE,CRect(nStartX,nStartY,nStartX+paramConfItem->m_nWidth,nStartY+DEFAULT_HEIGHT),this,m_nId++);
			nStartX += paramConfItem->m_nWidth;
			m_vtWnds.push_back(pParamNameWnd);

			CWnd* pParamWnd = NULL;
			if(paramConfItem->m_strCtrlType == "Edit")
			{
				pParamWnd = new CEdit();
				pParamWnd->Create("Edit",NULL,WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,CRect(nStartX,nStartY,nStartX+paramConfItem->m_nWidth1,nStartY+DEFAULT_HEIGHT),this,m_nId++);
			}
			else if(paramConfItem->m_strCtrlType == "Combobox")
			{
				std::vector<SComboItemConf>& vtComboItemConf = paramConfItem->m_vtComboConf;
				pParamWnd = new CComboBox();
				pParamWnd->Create("ComboBox",NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(nStartX,nStartY,nStartX+paramConfItem->m_nWidth1,nStartY+vtComboItemConf.size()*DEFAULT_HEIGHT),this,m_nId++);
				for (size_t comboItemIdx = 0; comboItemIdx < vtComboItemConf.size(); comboItemIdx++)
				{
					((CComboBox*)pParamWnd)->AddString(vtComboItemConf[comboItemIdx].m_strName.GetBuffer());
					if(vtComboItemConf[comboItemIdx].m_bChecked)
						((CComboBox*)pParamWnd)->SetCurSel(comboItemIdx);
				}
			}
			else if(paramConfItem->m_strCtrlType == "CheckCombo")
			{
				std::vector<SComboItemConf>& vtComboItemConf = paramConfItem->m_vtComboConf;
				CCheckComboBox* pCheckComboBox = new CCheckComboBox();
				pCheckComboBox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(nStartX,nStartY,nStartX+paramConfItem->m_nWidth1,nStartY+vtComboItemConf.size()*DEFAULT_HEIGHT), this, m_nId++);
				pParamWnd = pCheckComboBox;
				for (size_t comboItemIdx = 0; comboItemIdx < vtComboItemConf.size(); comboItemIdx++)
				{
					((CCheckComboBox*)pParamWnd)->AddString(vtComboItemConf[comboItemIdx].m_strName.GetBuffer());
					if(vtComboItemConf[comboItemIdx].m_bChecked)
						((CCheckComboBox*)pParamWnd)->SetCheck(comboItemIdx, true);
				}
			}

			nStartX += paramConfItem->m_nWidth1;
			m_vtWnds.push_back(pParamWnd);
			SPopParam popParam;
			popParam.m_pPopParamConf = &(*paramConfItem);
			popParam.m_pPopParamWnd = pParamWnd;
			popMain.m_vtPopParams.push_back(popParam);
		}

		if(mainConfItem->m_bNewLine)
		{
			nStartX = 0;
			nStartY += DEFAULT_HEIGHT;
		}

		m_vtPopMains.push_back(popMain);
	}

	return 0;
}

// CPopWindow 消息处理程序
afx_msg void CPopWindow::OnClose()
{
	CDialog::OnClose();
}

afx_msg void CPopWindow::OnRButtonDown(UINT nFlags, CPoint point)
{
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

void GetPopConf(lua_State* L, SPopConf& popConf)
{
	lua_getfield(L, -1, "ConfType");
	ASSERT(lua_type(L, -1) == LUA_TSTRING);
	popConf.m_strConfType = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "Confs");
	ASSERT(lua_type(L, -1) == LUA_TTABLE);

	//遍历主控件
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
	//读入lua配置
	lua_State* L = lua_open();
	luaL_dofile(L, name.c_str());

	//获取配置
	lua_getglobal(L, "config");
	ASSERT(lua_type(L, -1) == LUA_TTABLE);

	//遍历配置
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

	//关闭lua
	lua_close(L);
	return true;
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