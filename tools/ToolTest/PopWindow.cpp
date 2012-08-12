// PopWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTest.h"
#include "PopWindow.h"
#include "CheckComboBox.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

bool ValidPopMainStyle(const CString& style)
{
	if(style == "radio" || style == "checkbox")
		return true;
	else
		return false;
}

bool ValidPopParamStyle(const CString& style)
{
	if(style == "edit" || style == "combobox" || style == "comcheckbox")
		return true;
	else
		return false;
}

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
	for(std::vector<SPopItem*>::iterator iter1 = m_vtPopItems.begin(); iter1 != m_vtPopItems.end(); iter1++)
		delete (*iter1);

	for(std::vector<CWnd*>::iterator iter2 = m_vtWnds.begin(); iter2 != m_vtWnds.end(); iter2++)
		delete (*iter2);
}

void CPopWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPopWindow::CreatePopMain(SPopControlMainInfo* pMainInfo)
{
	ASSERT(pMainInfo);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	if(pMainInfo->m_strDlgStyle == "radio")
		dwStyle |= BS_AUTORADIOBUTTON;
	else if(pMainInfo->m_strDlgStyle == "checkbox")
		dwStyle |= BS_AUTOCHECKBOX;

	CButton* pMainCtrl = new CButton();
	pMainCtrl->Create(pMainInfo->m_strName.c_str(),dwStyle,CRect(pMainInfo->m_nStartWidth,pMainInfo->m_nStartHeight,pMainInfo->m_nStartWidth+pMainInfo->m_nWidth,pMainInfo->m_nStartHeight+20),this,m_nId++);
	m_vtWnds.push_back((CWnd*)pMainCtrl);
	SPopItem* pPopItem = new SPopItem();
	pPopItem->m_pMainWnd = (CWnd*)pMainCtrl;
	pPopItem->m_pMainInfo = pMainInfo;

	for(std::vector<SPopControlParamInfo*>::iterator paramIter = pMainInfo->m_vtParams.begin();
		paramIter != pMainInfo->m_vtParams.end();
		paramIter++)
	{
		CWnd* pParamWnd = NULL;
		SPopControlParamInfo* pParamInfo = *paramIter;
		int nNameL,nNameT,nNameR,nNameB;
		int nCtrlL,nCtrlT,nCtrlR,nCtrlB;
		nNameL = pParamInfo->m_nStartWidth;
		nNameT = pParamInfo->m_nStartHeight;
		nNameR = nNameL + pParamInfo->m_nWidth;
		nNameB = nNameT + DEFAULT_HEIGHT;
		nCtrlL = nNameR;
		nCtrlT = nNameT;
		nCtrlR = nCtrlL + pParamInfo->m_nWidth1;
		nCtrlB = nNameB;

		CStatic* pParamNameWnd = new CStatic();
		pParamNameWnd->Create(pParamInfo->m_strName.c_str(),WS_CHILD|WS_VISIBLE,CRect(nNameL,nNameT,nNameR,nNameB),this,m_nId++);
		pPopItem->m_vtParamWnds.push_back(pParamNameWnd);

		if(pParamInfo->m_strDlgStyle == "edit")
		{
			pParamWnd = new CEdit();
			pParamWnd->Create("Edit",NULL,WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,CRect(nCtrlL,nCtrlT,nCtrlR,nCtrlB),this,m_nId++);
		}
		else if(pParamInfo->m_strDlgStyle == "combobox")
		{
			nCtrlB += pParamInfo->m_mapValue.size() * DEFAULT_HEIGHT;
			pParamWnd = new CComboBox();
			pParamWnd->Create("ComboBox",NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(nCtrlL,nCtrlT,nCtrlR,nCtrlB),this,m_nId++);
			for (std::map<std::string,std::string>::iterator itemIter = pParamInfo->m_mapValue.begin();
				itemIter != pParamInfo->m_mapValue.end();
				itemIter++)
			{
				((CComboBox*)pParamWnd)->AddString(itemIter->first.c_str());
			}
		}
		else if(pParamInfo->m_strDlgStyle == "comcheckbox")
		{
			nCtrlB += pParamInfo->m_mapValue.size() * DEFAULT_HEIGHT;
			CCheckComboBox* pCheckComboBox = new CCheckComboBox();
			pCheckComboBox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(nCtrlL,nCtrlT,nCtrlR,nCtrlB), this, m_nId++);
			pParamWnd = pCheckComboBox;
			for (std::map<std::string,std::string>::iterator itemIter = pParamInfo->m_mapValue.begin();
				itemIter != pParamInfo->m_mapValue.end();
				itemIter++)
			{
				((CCheckComboBox*)pParamWnd)->AddString(itemIter->first.c_str());
			}
		}

		pPopItem->m_vtParamWnds.push_back(pParamWnd);
		pPopItem->m_mapParamInfos.insert(std::make_pair(pParamWnd->GetSafeHwnd(),pParamInfo));
	}
	m_vtPopItems.push_back(pPopItem);
}

BOOL CPopWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	int originDlgId = m_pOriginWnd->GetDlgCtrlID();
	if(g_mapOriginInfos.find(originDlgId) == g_mapOriginInfos.end())
		return TRUE;

	SOriginControlInfo& originInfo = g_mapOriginInfos[originDlgId];
	if(g_mapPopInfos.find(originInfo.m_strName) == g_mapPopInfos.end())
		return TRUE;

	CollectionPopControlInfosT& popInfo = g_mapPopInfos[originInfo.m_strName];
	for(CollectionPopControlInfosT::iterator mainIter = popInfo.begin(); mainIter != popInfo.end(); mainIter++)
		CreatePopMain(*mainIter);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CPopWindow, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CPopWindow::SetOrigin(CWnd* pWnd)
{
	m_pOriginWnd = pWnd;
}

BOOL CPopWindow::Show()
{
	DoModal();
	return TRUE;
}

// CPopWindow 消息处理程序
afx_msg void CPopWindow::OnClose()
{
	CDialog::OnClose();
}

//===================================================================================
CollectionOriginInfosT g_mapOriginInfos;

bool REG_PROPERTY(int PID, const char* name, int dlgStyle)
{
	ASSERT(g_mapOriginInfos.find(PID) == g_mapOriginInfos.end());
	SOriginControlInfo originInfo;
	originInfo.m_strName = name;
	originInfo.m_nDlgStyle = dlgStyle;
	g_mapOriginInfos.insert(std::make_pair(PID,originInfo));
	return true;
}

//===================================================================================
CollectionPopInfosT g_mapPopInfos;

//===================================================================================
bool LoadPopConfig(std::string name)
{
	rapidxml::xml_document<> XMLDoc;
	rapidxml::file<> fdoc(name.c_str());
	rapidxml::xml_node<>* XMLRoot;
	XMLDoc.parse<0>(fdoc.data());
	XMLRoot = XMLDoc.first_node("root");
	
	//遍历所有配置
	for(rapidxml::xml_node<>* propertyNode = XMLRoot->first_node(); propertyNode != NULL; propertyNode = propertyNode->next_sibling())
	{
		std::string propertyName = propertyNode->name();
		CollectionPopControlInfosT vtPopControlInfos;
		vtPopControlInfos.clear();
		int startHeight = 0;
		int startWidth = 0;
		for(rapidxml::xml_node<>* mainItem = propertyNode->first_node("item"); mainItem != NULL; mainItem = mainItem->next_sibling("item"))
		{
			SPopControlMainInfo* mainInfo = new SPopControlMainInfo();

			rapidxml::xml_node<>* nodeM;
			nodeM				= mainItem->first_node("dlgStyle");
			if(!nodeM)
				continue;

			CString dlgStyleM	= nodeM->value();					dlgStyleM.Trim();
			if(!ValidPopMainStyle(dlgStyleM))
				continue;
			mainInfo->m_strDlgStyle = dlgStyleM.GetBuffer();

			nodeM				= mainItem->first_node("name");
			CString nameM		= nodeM ? nodeM->value() : "";		nameM.Trim();ConvertUTF8ToGBK(nameM);
			mainInfo->m_strName	= nameM.GetBuffer();
			nodeM				= mainItem->first_node("value");
			CString valueM		= nodeM ? nodeM->value() : "0";		valueM.Trim();
			mainInfo->m_nValue	= atoi(valueM.GetBuffer());
			nodeM				= mainItem->first_node("checked");
			CString checkedM	= nodeM ? nodeM->value() : "0";		checkedM.Trim();
			mainInfo->m_bChecked = (bool)atoi(checkedM.GetBuffer());
			nodeM				= mainItem->first_node("nonewline");
			CString nonewlineM	= nodeM ? nodeM->value() : "0";		nonewlineM.Trim();
			mainInfo->m_bNonewline = (bool)atoi(nonewlineM.GetBuffer());
			nodeM				= mainItem->first_node("width");
			CString widthM		= nodeM ? nodeM->value() : MAIN_DEFAULT_WIDTH;	widthM.Trim();
			mainInfo->m_nWidth	= atoi(widthM.GetBuffer());
			nodeM				= mainItem->first_node("descr");
			CString descrM		= nodeM ? nodeM->value() : "";		descrM.Trim();ConvertUTF8ToGBK(descrM);
			mainInfo->m_strDescr	= descrM.GetBuffer();
			mainInfo->m_nHeight	= DEFAULT_HEIGHT;
			mainInfo->m_nStartWidth = startWidth;
			startWidth = startWidth + mainInfo->m_nWidth;
			mainInfo->m_nStartHeight = startHeight;

			for(rapidxml::xml_node<>* paramItem = mainItem->first_node("param"); paramItem != NULL; paramItem = paramItem->next_sibling("param"))
			{
				SPopControlParamInfo* paramInfo = new SPopControlParamInfo();

				rapidxml::xml_node<>* nodeP;
				nodeP				= paramItem->first_node("dlgStyle");
				if(!nodeP)
					continue;

				CString dlgStyleP	= nodeP->value();				dlgStyleP.Trim();
				if(!ValidPopParamStyle(dlgStyleP))
					continue;
				paramInfo->m_strDlgStyle = dlgStyleP.GetBuffer();
				for(rapidxml::xml_node<> * paramDlgItem = nodeP->first_node("item"); paramDlgItem != NULL; paramDlgItem = paramDlgItem->next_sibling())
				{
					rapidxml::xml_node<>* dlgItemNodeP;
					dlgItemNodeP = paramDlgItem->first_node("name");
					CString dlgItemNameP = dlgItemNodeP ? dlgItemNodeP->value() : ""; dlgItemNameP.Trim();ConvertUTF8ToGBK(dlgItemNameP);
					dlgItemNodeP = paramDlgItem->first_node("value");
					CString dlgItemValueP = dlgItemNodeP ? dlgItemNodeP->value() : ""; dlgItemValueP.Trim();
					paramInfo->m_mapValue.insert(std::make_pair(dlgItemNameP.GetBuffer(),dlgItemValueP.GetBuffer()));
				}

				nodeP				= paramItem->first_node("name");
				CString nameP		= nodeP ? nodeP->value() : "";	nameP.Trim();ConvertUTF8ToGBK(nameP);
				paramInfo->m_strName	= nameP.GetBuffer();
				nodeP				= paramItem->first_node("cname");
				CString cnameP		= nodeP ? nodeP->value() : "";	cnameP.Trim();
				paramInfo->m_strCName= cnameP.GetBuffer();
				nodeP				= paramItem->first_node("reverse");
				CString reverseP	= nodeP ? nodeP->value() : "";	reverseP.Trim();
				paramInfo->m_strReverse = reverseP.GetBuffer();
				nodeP				= paramItem->first_node("default");
				CString defaultP	= nodeP ? nodeP->value() : "";	defaultP.Trim();ConvertUTF8ToGBK(defaultP);
				paramInfo->m_strDefault = defaultP.GetBuffer();
				nodeP				= paramItem->first_node("descr");
				CString descrP		= nodeP ? nodeP->value() : "";	descrP.Trim();ConvertUTF8ToGBK(descrP);
				paramInfo->m_strDescr = descrP.GetBuffer();
				nodeP				= paramItem->first_node("width");
				CString widthP		= nodeP ? nodeP->value() : PARAM_DEFAULT_WIDTH;	 widthP.Trim();
				paramInfo->m_nWidth	= atoi(widthP.GetBuffer());
				nodeP				= paramItem->first_node("width1");
				CString width1P		= nodeP ? nodeP->value() : PARAM_DEFAULT_WIDTH1; width1P.Trim();
				paramInfo->m_nWidth1	= atoi(width1P.GetBuffer());
				paramInfo->m_nHeight = DEFAULT_HEIGHT;
				paramInfo->m_nHeight1= DEFAULT_HEIGHT;
				paramInfo->m_nStartWidth = startWidth;
				startWidth = startWidth + paramInfo->m_nWidth + paramInfo->m_nWidth1;
				paramInfo->m_nStartHeight= startHeight;
				mainInfo->m_vtParams.push_back(paramInfo);
			}

			if((!mainInfo->m_vtParams.empty()) || (!mainInfo->m_bNonewline))
			{
				startWidth = 0;
				startHeight += DEFAULT_HEIGHT + DEFAULT_HEIGHT_INTER;
			}

			vtPopControlInfos.push_back(mainInfo);
		}
		g_mapPopInfos.insert(std::make_pair(propertyName,vtPopControlInfos));
	}

	return true;
}

void DeclareNo(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
}

void DeclareSourceListBoxDefType(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
}

void DeclareSourceListBoxInt(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
}

void DeclareSourceEditInt(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, long DEFVAL)
{
}

void DeclareSourceEditStr(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DeclareSourceEditDouble(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, float DEFVAL)
{
}

void DeclareSourceEditDefType(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DeclareSourceEditDefTypeAndParams(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DeclareSourceTupleEditDefType(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DeclareSourceCheckBox(lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
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