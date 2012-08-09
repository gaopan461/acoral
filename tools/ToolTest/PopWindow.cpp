// PopWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTest.h"
#include "PopWindow.h"
#include "CheckComboBox.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

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
	LoadConfig();
	return TRUE;
}

void CPopWindow::LoadConfig()
{
	rapidxml::xml_document<> XMLDoc;
	rapidxml::file<> fdoc("ToolTestCfg.xml");
	rapidxml::xml_node<>* XMLRoot;
	XMLDoc.parse<0>(fdoc.data());
	XMLRoot = XMLDoc.first_node("root");
	rapidxml::xml_node<>* node = XMLRoot->first_node("testRadio");
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