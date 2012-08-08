// PopWindow.cpp : ʵ���ļ�
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
// CPopWindow �Ի���

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

// CPopWindow ��Ϣ�������
afx_msg void CPopWindow::OnClose()
{
	CCheckComboBox* check = (CCheckComboBox*)m_pOriginWnd;
	CString str = check->GetCheckedIDs();
	MessageBox(str.GetBuffer(0));
	CDialog::OnClose();
}

//===================================================================================
CollectionPropertyInfosT g_mapPropertyMaps;

bool REG_PROPERTY(int PID, const char* name, int dlgStyle)
{
	ASSERT(g_mapPropertyMaps.find(PID) == g_mapPropertyMaps.end());
	SCustonPropertyOriginInfo propertyInfo;
	propertyInfo.name = name;
	propertyInfo.dlgStyle = dlgStyle;
	g_mapPropertyMaps.insert(std::make_pair(PID,propertyInfo));
	return true;
}