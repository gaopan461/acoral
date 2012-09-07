// ToolTestDlg.h : 头文件
//

#pragma once
#include "PopWindow.h"
#include "afxwin.h"
#include "ToolBase.h"
#include "ToolTree.h"

using namespace actools;

// CToolTestDlg 对话框
class CToolTestDlg : public CDialog, public ToolBase
{
	HANDLE_POP_CTRL
// 构造
public:
	CToolTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CToolTestDlg();

// 对话框数据
	enum { IDD = IDD_TOOLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitCommandMap();
	afx_msg void OnBnClickedSavetodb();
	afx_msg void OnBnClickedLoadfromdb();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	ToolTree m_objMainTree;
};
