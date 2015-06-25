// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PiggyStressTestClient.h"
#include "MainDlg.h"

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


// CMainDlg �Ի���




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


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
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

	// ��ʼ��������Ϣ
	this->InitGUI();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainDlg::OnPaint()
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////
// ��ʼ��������Ϣ
void CMainDlg::InitGUI()
{
	// ��ʼ��Socket��
	if( false==m_Client.LoadSocketLib() )
	{
		AfxMessageBox(_T("����Winsock 2.2ʧ�ܣ����������޷����У�"));
		PostQuitMessage(0);
	}

	// ���ñ���IP��ַ
	SetDlgItemText( IDC_IPADDRESS_SERVER,m_Client.GetLocalIP() );
	// ����Ĭ�϶˿�
	SetDlgItemInt( IDC_EDIT_PORT,DEFAULT_PORT );
	// ����Ĭ�ϵĲ����߳���
	SetDlgItemInt(IDC_EDIT_THREADS,DEFAULT_THREADS);
	// ����Ĭ�Ϸ�����Ϣ
	SetDlgItemText(IDC_EDIT_MESSAGE,DEFAULT_MESSAGE);
	
	// ��ʼ���б�
	this->InitListCtrl();

	// ����������ָ��
	m_Client.SetMainDlg(this);
}


///////////////////////////////////////////////////////////////////////
//	��ʼ��List Control
void CMainDlg::InitListCtrl()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
	pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList->InsertColumn(0,"INFORMATION",LVCFMT_LEFT,500);
}

///////////////////////////////////////////////////////////////////////
// ��ʼ����
void CMainDlg::OnBnClickedOk()
{	
	int nPort = GetDlgItemInt(IDC_EDIT_PORT);
	int nThreads = GetDlgItemInt(IDC_EDIT_THREADS);
	CString strIP,strMessage;
	GetDlgItemText(IDC_IPADDRESS_SERVER,strIP);
	GetDlgItemText(IDC_EDIT_MESSAGE,strMessage);
	if( strIP==_T("") || strMessage==_T("") || nPort<=0 || nThreads<=0 )
	{
		AfxMessageBox(_T("������Ϸ��Ĳ�����"));
		return;
	}

	// ��CClient���ò���
	m_Client.SetIP(strIP);
	m_Client.SetPort(nPort);
	m_Client.SetThreads(nThreads);
	m_Client.SetMessage(strMessage);

	// ��ʼ
	if( !m_Client.Start() )
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
		return;
	}

	AddInformation(_T("���Կ�ʼ"));
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}


//////////////////////////////////////////////////////////////////////
//	ֹͣ����
void CMainDlg::OnBnClickedStop()
{
	AddInformation(_T("����ֹͣ"));
	// ֹͣ
	m_Client.Stop();

	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

/////////////////////////////////////////////////////////////////////
//	�˳�
void CMainDlg::OnBnClickedCancel()
{
	// ֹͣ����
	m_Client.Stop();

	// ж��Socket��
	m_Client.UnloadSocketLib();

	OnCancel();
}

//////////////////////////////////////////////////////////////////////
//	�Ի�������ʱ�������ͷ���Դ
void CMainDlg::OnDestroy()
{
	OnBnClickedCancel();

	CDialog::OnDestroy();
}
