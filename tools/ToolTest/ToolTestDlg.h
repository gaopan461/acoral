// ToolTestDlg.h : ͷ�ļ�
//

#pragma once
#include "checkcombobox.h"
#include "PopWindow.h"
#include "afxwin.h"


// CToolTestDlg �Ի���
class CToolTestDlg : public CDialog
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
	CListBox m_test1;
};
