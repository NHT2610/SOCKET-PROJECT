// Login.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "Login.h"
#include "MainDialog.h"
#include "SignUp.h"
#include "ClientSocket.h"

// Login dialog

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, username(_T(""))
	, password(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, USERNAME, username);
	DDX_Text(pDX, PASSWORD, password);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(LOGIN, &Login::OnBnClickedLogin)
	ON_BN_CLICKED(LOGIN, &Login::OnBnClickedLogin)
	ON_BN_CLICKED(SIGNUP1, &Login::OnBnClickedSignup1)
END_MESSAGE_MAP()


// Login message handlers

BOOL Login::OnInitDialog()
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Login::OnPaint()
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
HCURSOR Login::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Hàm kiểm tra định dạng username
int Login::checkUsername(CString& name) {
	int len = int(name.GetLength());
	if (!len) { return 1; }
	char SpecialCharacters[] = { '~','`','!','@','#','$','%','^','&','*','(',')','-','_',
		'=','+','{','}','[',']','|',':',';','"',',','<','>','.','?','/' };
	int i = 0;
	int size = int(sizeof(SpecialCharacters) / sizeof(char));
	while (i < len) {
		for (int j = 0; j < size; ++j) {
			if (name[i] == SpecialCharacters[j]) { return 2; }
		}
		++i;
	}
	return 0;
}

//Hàm kiểm tra định dạng password
int Login::checkPassword(CString& pass) {
	int len = int(pass.GetLength());
	if (!len) { return 1; }
	int i = 0;
	while (i < len) {
		if (pass[i] == ' ') { return 2; }
		++i;
	}
	return 0;
}

//Hàm xử lý khi click vào nút ĐĂNG NHẬP
void Login::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(USERNAME, username);
	GetDlgItemText(PASSWORD, password);
	if (checkUsername(username) == 1) {
		MessageBox(L"Ban CHUA dien Ten dang nhap!", L"Error", MB_OK | MB_ICONERROR);
	}
	else if (checkUsername(username) == 2) {
		MessageBox(L"Ten dang nhap KHONG duoc chua khoang trang!", L"Error", MB_OK | MB_ICONERROR);
	}
	if (checkPassword(password) == 1) {
		MessageBox(L"Ban CHUA dien Mat khau!", L"Error", MB_OK | MB_ICONERROR);
	}
	else if (checkPassword(password) == 2) {
		MessageBox(L"Mat khau KHONG duoc chua khoang trang!", L"Error", MB_OK | MB_ICONERROR);
	}
	if (!checkUsername(username) && !checkPassword(password)) {
		int check_login = client_socket.login(username, password, _T("LOGIN"));
		if (check_login == 1) {
			MessageBox(L"Ten dang nhap KHONG ton tai!", L"Error", MB_OK | MB_ICONERROR);
		}
		else if (check_login == 2) {
			MessageBox(L"Mat khau KHONG dung!", L"Error", MB_OK | MB_ICONERROR);
		}
		else {
			MainDialog main;
			main.DoModal();
		}
	}
}

//Hàm xử lý khi click vào nút ĐĂNG KÝ của Dialog Login
void Login::OnBnClickedSignup1()
{
	// TODO: Add your control notification handler code here
	SignUp new_register;
	new_register.DoModal();
}
