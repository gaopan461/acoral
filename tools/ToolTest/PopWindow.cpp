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
	if(style == "edit" || style == "combox" || style == "comcheckbox")
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
{

}

CPopWindow::~CPopWindow()
{
}

void CPopWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CPopWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
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
	CCheckComboBox* check = (CCheckComboBox*)m_pOriginWnd;
	CString str = check->GetCheckedIDs();
	MessageBox(str.GetBuffer(0));
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
		MessageBox(NULL,propertyName.c_str(),NULL,MB_OK);
		for(rapidxml::xml_node<>* mainItem = propertyNode->first_node("item"); mainItem != NULL; mainItem = mainItem->next_sibling("item"))
		{
			SPopControlMainInfo mainInfo;

			rapidxml::xml_node<>* nodeM;
			nodeM				= mainItem->first_node("dlgStyle");
			if(!nodeM)
				continue;

			CString dlgStyleM	= nodeM->value();					dlgStyleM.Trim();
			if(!ValidPopMainStyle(dlgStyleM))
				continue;
			mainInfo.m_strDlgStyle = dlgStyleM.GetBuffer();

			nodeM				= mainItem->first_node("name");
			CString nameM		= nodeM ? nodeM->value() : "";		nameM.Trim();ConvertUTF8ToGBK(nameM);
			mainInfo.m_strName	= nameM.GetBuffer();
			nodeM				= mainItem->first_node("value");
			CString valueM		= nodeM ? nodeM->value() : "0";		valueM.Trim();
			mainInfo.m_nValue	= atoi(valueM.GetBuffer());
			nodeM				= mainItem->first_node("checked");
			CString checkedM	= nodeM ? nodeM->value() : "0";		checkedM.Trim();
			mainInfo.m_bChecked = (bool)atoi(checkedM.GetBuffer());
			nodeM				= mainItem->first_node("nonewline");
			CString nonewlineM	= nodeM ? nodeM->value() : "0";		nonewlineM.Trim();
			mainInfo.m_bNonewline = (bool)atoi(nonewlineM.GetBuffer());
			nodeM				= mainItem->first_node("width");
			CString widthM		= nodeM ? nodeM->value() : MAIN_DEFAULT_WIDTH;	widthM.Trim();
			mainInfo.m_nWidth	= atoi(widthM.GetBuffer());
			nodeM				= mainItem->first_node("descr");
			CString descrM		= nodeM ? nodeM->value() : "";		descrM.Trim();
			mainInfo.m_strDescr	= descrM.GetBuffer();
			mainInfo.m_nHeight	= DEFAULT_HEIGHT;
			mainInfo.m_nStartWidth = 0;
			mainInfo.m_nStartHeight = -1;

			for(rapidxml::xml_node<>* paramItem = propertyNode->first_node("param"); paramItem != NULL; paramItem = paramItem->next_sibling("param"))
			{
				SPopControlParamInfo paramInfo;

				rapidxml::xml_node<>* nodeP;
				nodeP				= paramItem->first_node("dlgStyle");
				if(!nodeP)
					continue;

				CString dlgStyleP	= nodeP->value();				dlgStyleP.Trim();
				if(!ValidPopParamStyle(dlgStyleP))
					continue;
				paramInfo.m_strDlgStyle = dlgStyleP.GetBuffer();

				nodeP				= paramItem->first_node("name");
				CString nameP		= nodeP ? nodeP->value() : "";	nameP.Trim();ConvertUTF8ToGBK(nameP);
				paramInfo.m_strName	= nameP.GetBuffer();
				nodeP				= paramItem->first_node("cname");
				CString cnameP		= nodeP ? nodeP->value() : "";	cnameP.Trim();
				paramInfo.m_strCName= cnameP.GetBuffer();
				nodeP				= paramItem->first_node("reverse");
				CString reverseP	= nodeP ? nodeP->value() : "";	reverseP.Trim();
				paramInfo.m_strReverse = reverseP.GetBuffer();
				nodeP				= paramItem->first_node("default");
				CString defaultP	= nodeP ? nodeP->value() : "";	defaultP.Trim();
				paramInfo.m_strDefault = defaultP.GetBuffer();
				nodeP				= paramItem->first_node("descr");
				CString descrP		= nodeP ? nodeP->value() : "";	descrP.Trim();
				paramInfo.m_strDescr = descrP.GetBuffer();
				nodeP				= paramItem->first_node("width");
				CString widthP		= nodeP ? nodeP->value() : PARAM_DEFAULT_WIDTH;	 widthP.Trim();
				paramInfo.m_nWidth	= atoi(widthP.GetBuffer());
				nodeP				= paramItem->first_node("width1");
				CString width1P		= nodeP ? nodeP->value() : PARAM_DEFAULT_WIDTH1; width1P.Trim();
				paramInfo.m_nWidth1	= atoi(width1P.GetBuffer());
				paramInfo.m_nHeight = DEFAULT_HEIGHT;
				paramInfo.m_nHeight1= DEFAULT_HEIGHT;
				paramInfo.m_nStartWidth = -1;
				paramInfo.m_nStartHeight= -1;
			}
		}
	}

	return true;
}

void DECLARE_NO(bool ISWRITETODB, int DLGID, const char* NAME)
{
}

void DECLARE_SOURCE_LISTBOX_DEFTYPE(bool ISWRITETODB, int DLGID, const char* NAME)
{
}

void DECLARE_SOURCE_LISTBOX_INT(bool ISWRITETODB, int DLGID, const char* NAME)
{
}

void DECLARE_SOURCE_EDIT_INT(bool ISWRITETODB, int DLGID, const char* NAME, long DEFVAL)
{
}

void DECLARE_SOURCE_EDIT_STR(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DECLARE_SOURCE_EDIT_DOUBLE(bool ISWRITETODB, int DLGID, const char* NAME, float DEFVAL)
{
}

void DECLARE_SOURCE_EDIT_DEFTYPE(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DECLARE_SOURCE_EDIT_DEFTYPE_AND_PARAMS(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DECLARE_SOURCE_TUPLEEDIT_DEFTYPE(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
}

void DECLARE_SOURCE_CHECKBOX(bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
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