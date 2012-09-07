// ToolTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ToolTest.h"
#include "ToolTestDlg.h"
#include "ac_lua.h"
#include "ToolBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CToolTestDlg �Ի���




CToolTestDlg::CToolTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolTestDlg::IDD, pParent)
{
	//_CrtSetBreakAlloc(229);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CToolTestDlg::~CToolTestDlg()
{
	DeInitTool();
}

void CToolTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SAVETODB, &CToolTestDlg::OnBnClickedSavetodb)
	ON_BN_CLICKED(IDC_LOADFROMDB, &CToolTestDlg::OnBnClickedLoadfromdb)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CToolTestDlg ��Ϣ�������

BOOL CToolTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// Get log fonts
	LOGFONT lfFont, lfDefaultFont;
	GetFont()->GetLogFont(&lfFont);
	GetFont()->GetLogFont(&lfDefaultFont);
	_tcscpy(lfDefaultFont.lfFaceName, _T("Arial"));

	//����������10msһ��
	::SetTimer(m_hWnd, 1, 10, NULL);

	//for test
	::SetTimer(m_hWnd, 2, 1000, NULL);

	//��ʼ������
	InitTool("ToolTest.txt",GetDlgItem(IDC_LOG)->m_hWnd);

	INFO_MSG("-------------------------------------------\n");
	INFO_MSG("              ToolTest start               \n");
	INFO_MSG("-------------------------------------------\n");

	//���������ؼ�
	m_objMainTree.Create(CRect(10,10,180,340), this, IDC_TREE_MAIN);

	//��������
	LoadPopConfig("ToolTestCfg.lua");
	INFO_MSG("Load pop config success\n");

	//��ʼ���ؼ������õĹ���
	InitCommandMap();
	INFO_MSG("Init command map success\n");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CToolTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CToolTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CToolTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CToolTestDlg::InitCommandMap()
{
	REG_PROPERTY(IDC_EDIT1,"TestRadio2");
	REG_PROPERTY(IDC_LIST1,"TestCheck2");
}


void CToolTestDlg::OnBnClickedSavetodb()
{
	lua_newtable(m_pLua);
	MainToDB(GetDlgItem(IDC_EDIT1),m_pLua,"Edit1");
	MainToDB(GetDlgItem(IDC_LIST1),m_pLua,"List1");
	acutils::LuaUtils::SaveTableToFile(m_pLua, "datas", "data.lua");
	lua_pop(m_pLua, 1);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CToolTestDlg::OnBnClickedLoadfromdb()
{
	luaL_dofile(m_pLua, "data.lua");
	lua_getglobal(m_pLua, "datas");
	DBToMain(m_pLua, "Edit1", GetDlgItem(IDC_EDIT1));
	DBToMain(m_pLua, "List1", GetDlgItem(IDC_LIST1));
	lua_pop(m_pLua, 1);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CToolTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//������
	if(nIDEvent == 1)
	{
		this->Update();
	}
	if (nIDEvent == 2)
	{
		DEBUG_MSG("-----------���------------\n");
	}

	CDialog::OnTimer(nIDEvent);
}
