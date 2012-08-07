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
CollectionPropertyInfosT g_mapPropertyMaps;

bool REG_PROPERTY(int PID, const char* name, int dlgStyle, const char* sourcename)
{
	ASSERT(g_mapPropertyMaps.find(PID) == g_mapPropertyMaps.end());
	SCustonPropertyOriginInfo propertyInfo;
	propertyInfo.name = name;
	propertyInfo.dlgStyle = dlgStyle;
	propertyInfo.sourceName = sourcename;
	g_mapPropertyMaps.insert(std::make_pair(PID,propertyInfo));
	return true;
}