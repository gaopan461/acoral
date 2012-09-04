// ToolTestDlg.h : ͷ�ļ�
//

#pragma once
#include "PopWindow.h"
#include "afxwin.h"
#include "ToolBase.h"


// CToolTestDlg �Ի���
class CToolTestDlg : public CDialog, public actools::ToolBase
{
	HANDLE_POP_CTRL
// ����
public:
	CToolTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CToolTestDlg();

// �Ի�������
	enum { IDD = IDD_TOOLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitCommandMap();
	afx_msg void OnBnClickedSavetodb();
	afx_msg void OnBnClickedLoadfromdb();
private:
	lua_State* m_pLua;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
