
// ServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg dialog



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, SERVERINFO, ServerActivity);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(DISCONNECT, &CServerDlg::OnBnClickedStop)
	ON_BN_CLICKED(START, &CServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(STOPSERVER, &CServerDlg::OnBnClickedStopserver)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//Snippet 01: Get Client Coordinate of the Rectangle
		CRect ClinetRect;
		this->GetClientRect(&ClinetRect);

		//Snippet 02: Declare Device Contexts and bitmap
		CDC memoryDC;
		CPaintDC DialogDC(this);
		CBitmap tiledImage;

		//Snippet 03: Load Image to Memory
		memoryDC.CreateCompatibleDC(&DialogDC);
		tiledImage.LoadBitmap(IDB_BITMAP1);
		memoryDC.SelectObject(&tiledImage);

		//Snippet 04: Copy memory pixels to dialog surface
		DialogDC.BitBlt(0, 0, ClinetRect.Width(), ClinetRect.Height(), &memoryDC, 0, 0, SRCCOPY);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CServerDlg::OnBnClickedStop()
//{
//	// TODO: Add your control notification handler code here
//	m_pServerSock->ServerSend("DISCONNECT");
//	ShowServerInfo("<<< Server da ngung ket noi cua Client");
//	m_pServerSock->ConnectorClose();
//}

//Khởi tạo kết nối, cho phép client kết nối đến server
void CServerDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	//Khởi tạo server
	CSocket Server;
	CSocket Client;
	DWORD ThreadID;
	HANDLE ThreadStatus;
	int count = 1;
	if (!Server.Create(12345)) {
		MessageBox(L"Khởi tạo server THẤT BẠI!", L"Information", MB_OK | MB_ICONINFORMATION);
		exit(1);
	}
	ShowServerInfo("DANG DOI KET NOI ....\r\n");
	try
	{
		while (count <= 5) {
			Server.Listen();
			if (Server.Accept(Client)) {
				m_pServerSock[count] = new ServerManager(this);
				ShowServerInfo(">>>Client " + to_string(count) + " da ket noi\r\n");
				m_pServerSock[count]->hConnected = new SOCKET();
				*m_pServerSock[count]->hConnected = Client.Detach();
				m_pServerSock[count]->CountThread = count;
				ThreadStatus = CreateThread
				(NULL, 0, ThreadProccess, m_pServerSock[count], 0, &ThreadID);
			}
			++count;
		}
	}
	catch (const std::exception&)
	{
		MessageBox(L"Client mat ket noi!", L"Information", MB_OK | MB_ICONINFORMATION);
	}
	Server.Close();
}


void CServerDlg::AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText) {
	int nLength = edit.GetWindowTextLength();
	edit.SetSel(nLength, nLength);
	edit.ReplaceSel(pszText);
}

//Hiển thị các hoạt động ra hộp Server Info
void CServerDlg::ShowServerInfo(string sValue) {
	CString strLine(sValue.c_str());
	AppendTextToEditCtrl(ServerActivity, strLine);
}

void CServerDlg::OnBnClickedStopserver()
{
	// TODO: Add your control notification handler code here
	puts("<<< Server STOP!");
	ShowServerInfo("<<< Server STOP!");
	
	exit(1);
}
