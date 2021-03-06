// ToolTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTest.h"
#include "ToolTestDlg.h"
#include "ac_lua.h"
#include "ToolBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CToolTestDlg 对话框




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


// CToolTestDlg 消息处理程序

BOOL CToolTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// Get log fonts
	LOGFONT lfFont, lfDefaultFont;
	GetFont()->GetLogFont(&lfFont);
	GetFont()->GetLogFont(&lfDefaultFont);
	_tcscpy(lfDefaultFont.lfFaceName, _T("Arial"));

	//设置心跳，10ms一次
	::SetTimer(m_hWnd, 1, 10, NULL);

	//for test
	::SetTimer(m_hWnd, 2, 1000, NULL);

	//初始化工具
	InitTool("ToolTest.txt",GetDlgItem(IDC_LOG)->m_hWnd);

	INFO_MSG("-------------------------------------------\n");
	INFO_MSG("              ToolTest start               \n");
	INFO_MSG("-------------------------------------------\n");

	//创建主树控件
	m_objMainTree.Create(CRect(10,10,180,340), this, IDC_TREE_MAIN);

	//载入配置
	LoadPopConfig("ToolTestCfg.lua");
	INFO_MSG("Load pop config success\n");

	//初始化控件和配置的关联
	InitCommandMap();
	INFO_MSG("Init command map success\n");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CToolTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加控件通知处理程序代码
}

void CToolTestDlg::OnBnClickedLoadfromdb()
{
	luaL_dofile(m_pLua, "data.lua");
	lua_getglobal(m_pLua, "datas");
	DBToMain(m_pLua, "Edit1", GetDlgItem(IDC_EDIT1));
	DBToMain(m_pLua, "List1", GetDlgItem(IDC_LIST1));
	lua_pop(m_pLua, 1);
	// TODO: 在此添加控件通知处理程序代码
}

void CToolTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//心跳到
	if(nIDEvent == 1)
	{
		this->Update();
	}
	if (nIDEvent == 2)
	{
		DEBUG_MSG("-----------你好------------\n");
	}

	CDialog::OnTimer(nIDEvent);
}
