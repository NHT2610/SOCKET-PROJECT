// SignUp.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "SignUp.h"
#include "ClientSocket.h"


// SignUp dialog

IMPLEMENT_DYNAMIC(SignUp, CDialogEx)

SignUp::SignUp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, SignupUsername(_T(""))
	, SignupPassword(_T(""))
	, RePassword(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

SignUp::~SignUp()
{
}

void SignUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, USERNAME_SIGNUP, SignupUsername);
	DDX_Text(pDX, PASSWORD_SIGNUP, SignupPassword);
	DDX_Text(pDX, REPASSWORD, RePassword);
}


BEGIN_MESSAGE_MAP(SignUp, CDialogEx)
	ON_BN_CLICKED(SIGNUP2, &SignUp::OnBnClickedSignup2)
END_MESSAGE_MAP()


// SignUp message handlers

BOOL SignUp::OnInitDialog()
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
	SetIcon(hIcon, FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void SignUp::OnPaint()
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
HCURSOR SignUp::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Hàm kiểm tra định dạng username
int SignUp::checkUsername(CString& name) {
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
int SignUp::checkPassword(CString& pass) {
	int len = int(pass.GetLength());
	if (!len) { return 1; }
	int i = 0;
	while (i < len) {
		if (pass[i] == ' ' || pass[i] == '|') { return 2; }
		++i;
	}
	return 0;
}

//Hàm kiểm tra việc nhập lại password
bool SignUp::checkRePass(CString& pass, CString& repass) {
	if (repass.GetLength() == 0 || pass != repass) { return FALSE; }
	return TRUE;
}

//Hàm xử lý khi click vào nút ĐĂNG KÝ của Dialog SignUP
void SignUp::OnBnClickedSignup2()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(USERNAME_SIGNUP, SignupUsername);
	GetDlgItemText(PASSWORD_SIGNUP, SignupPassword);
	GetDlgItemText(REPASSWORD, RePassword);
	if (checkUsername(SignupUsername) == 1) {
		MessageBox(L"Ban CHUA dien Ten dang nhap!", L"Error", MB_OK | MB_ICONERROR);
	}
	else if (checkUsername(SignupUsername) == 2) {
		MessageBox(L"Ten dang nhap KHONG duoc chua khoang trang!", L"Error", MB_OK | MB_ICONERROR);
	}
	if (checkPassword(SignupPassword) == 1) {
		MessageBox(L"Ban CHUA dien Mat khau!", L"Error", MB_OK | MB_ICONERROR);
	}
	else if (checkPassword(SignupPassword) == 2) {
		MessageBox(L"Mat khau KHONG duoc chua khoang trang hoac ky tu '|'!", L"Error", MB_OK | MB_ICONERROR);
	}
	if (!checkRePass(SignupPassword, RePassword)) {
		MessageBox(L"Mat khau nhap lai KHONG khop!", L"Error", MB_OK | MB_ICONERROR);
	}
	if (!checkUsername(SignupUsername) && !checkPassword(SignupPassword)
		&& checkRePass(SignupPassword, RePassword))
	{
		int checkRegister = client_socket.Register(SignupUsername, SignupPassword, _T("REGISTER"));
		if (checkRegister == 1) {
			MessageBox(L"Ten dang nhap da ton tai!", L"Information", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(L"Dang ky THANH CONG!", L"Information", MB_OK | MB_ICONINFORMATION);
		}
	}
}
