// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PiggyStressTestClient.h"
#include "MainDlg.h"

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


// CMainDlg 对话框




CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_STOP, &CMainDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
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

	// 初始化界面信息
	this->InitGUI();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMainDlg::OnPaint()
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////
// 初始化界面信息
void CMainDlg::InitGUI()
{
	// 初始化Socket库
	if( false==m_Client.LoadSocketLib() )
	{
		AfxMessageBox(_T("加载Winsock 2.2失败，服务器端无法运行！"));
		PostQuitMessage(0);
	}

	// 设置本机IP地址
	SetDlgItemText( IDC_IPADDRESS_SERVER,m_Client.GetLocalIP() );
	// 设置默认端口
	SetDlgItemInt( IDC_EDIT_PORT,DEFAULT_PORT );
	// 设置默认的并发线程数
	SetDlgItemInt(IDC_EDIT_THREADS,DEFAULT_THREADS);
	// 设置默认发送信息
	SetDlgItemText(IDC_EDIT_MESSAGE,DEFAULT_MESSAGE);
	
	// 初始化列表
	this->InitListCtrl();

	// 设置主界面指针
	m_Client.SetMainDlg(this);
}


///////////////////////////////////////////////////////////////////////
//	初始化List Control
void CMainDlg::InitListCtrl()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList->InsertColumn(0,"INFORMATION",LVCFMT_LEFT,500);
}

///////////////////////////////////////////////////////////////////////
// 开始测试
void CMainDlg::OnBnClickedOk()
{	
	int nPort = GetDlgItemInt(IDC_EDIT_PORT);
	int nThreads = GetDlgItemInt(IDC_EDIT_THREADS);
	CString strIP,strMessage;
	GetDlgItemText(IDC_IPADDRESS_SERVER,strIP);
	GetDlgItemText(IDC_EDIT_MESSAGE,strMessage);
	if( strIP==_T("") || strMessage==_T("") || nPort<=0 || nThreads<=0 )
	{
		AfxMessageBox(_T("请输入合法的参数！"));
		return;
	}

	// 给CClient设置参数
	m_Client.SetIP(strIP);
	m_Client.SetPort(nPort);
	m_Client.SetThreads(nThreads);
	m_Client.SetMessage(strMessage);

	// 开始
	if( !m_Client.Start() )
	{
		AfxMessageBox(_T("启动失败！"));
		return;
	}

	AddInformation(_T("测试开始"));
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}


//////////////////////////////////////////////////////////////////////
//	停止测试
void CMainDlg::OnBnClickedStop()
{
	AddInformation(_T("测试停止"));
	// 停止
	m_Client.Stop();

	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

/////////////////////////////////////////////////////////////////////
//	退出
void CMainDlg::OnBnClickedCancel()
{
	// 停止监听
	m_Client.Stop();

	// 卸载Socket库
	m_Client.UnloadSocketLib();

	OnCancel();
}

//////////////////////////////////////////////////////////////////////
//	对话框销毁时，彻底释放资源
void CMainDlg::OnDestroy()
{
	OnBnClickedCancel();

	CDialog::OnDestroy();
}
