
// ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#include "Login.h"

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


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IP_SERVER, IP_server);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(IPN_FIELDCHANGED, IP_SERVER, &CClientDlg::OnIpnFieldchangedServer)
	ON_BN_CLICKED(CONNECT_BUUTON, &CClientDlg::OnBnClickedBuuton)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnIpnFieldchangedServer(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

//Hàm kiểm tra việc nhập IP
bool CClientDlg::checkIP(CString& ip) {
	if (!ip.GetLength() || !isdigit(ip[0])) { return FALSE; }
	int a = -1, b = -1, c = -1, d = -1;
	int count = 1;
	int j;
	CString temp;
	for (int i = 0; i < int(ip.GetLength()); ++i) {
		for (j = i; j < int(ip.GetLength()) && ip[j] != '.'; ++j) {
			if (isdigit(ip[j])) { temp += ip[j]; }
			else { return FALSE; }
		}
		switch (count)
		{
		case 1: a = _wtoi(temp); break;
		case 2: b = _wtoi(temp); break;
		case 3: c = _wtoi(temp); break;
		default: d = _wtoi(temp); break;
		}
		++count;
		temp = "";
		i = j;
	}
	if (a < 0 || b < 0 || c < 0 || d < 0) { return FALSE; }
	return TRUE;
}

void CClientDlg::OnBnClickedBuuton()
{
	// TODO: Add your control notification handler code here
	CSocket client_socket;
	if (!client_socket.Create()) {
		MessageBox(L"Khoi tao socket THAT BAI!", L"Warning", MB_OK | MB_ICONWARNING);
		exit(1);
	}
	ipAddress = _T("");
	GetDlgItem(IP_SERVER)->GetWindowTextW(ipAddress);
	if (IP_server.IsBlank() || !checkIP(ipAddress)) {
		MessageBox(L"Vui long kiem tra lai IP!", L"Error", MB_OK | MB_ICONERROR);
	}
	else {
		if (!client_socket.Connect(ipAddress, 12345)) {
			MessageBox(L"Ket noi den server THAT BAI!", L"Infomation", MB_OK | MB_ICONINFORMATION);
		}
		else {
			Login new_login;
			INT_PTR button1 = new_login.DoModal();
			/*if (button1 == LOGIN) {
				new_login.OnBnClickedLogin();
			}
			if (button1 == SIGNUP1) {

			}*/
		}
	}
}
