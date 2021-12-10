// login.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "login.h"
#include "SignUp.h"
#include "MainDialog.h"


// login dialog

IMPLEMENT_DYNAMIC(login, CDialogEx)

login::login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, username(_T(""))
	, password(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

login::~login()
{
}

void login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, USERNAME, username);
	DDX_Text(pDX, PASSWORD, password);
}


BEGIN_MESSAGE_MAP(login, CDialogEx)
	ON_BN_CLICKED(SIGNUP, &login::OnBnClickedSignup)
	ON_BN_CLICKED(SIGNIN, &login::OnBnClickedSignin)
END_MESSAGE_MAP()


// login message handlers


void login::OnBnClickedSignup()
{
	// TODO: Add your control notification handler code here
	SignUp new_acc;
	new_acc.DoModal();
}

//Hàm kiểm tra định dạng username
int login::checkUsername(CString& name) {
	int len = int(name.GetLength());
	if (!len) { return 1; }
	int i = 0;
	while (i < len) {
		if (name[i] == ' ') { return 2; }
	}
	return 0;
}

//Hàm kiểm tra định dạng password
int login::checkPassword(CString& pass) {
	int len = int(pass.GetLength());
	if (!len) { return 1; }
	int i = 0;
	while (i < len) {
		if (pass[i] == ' ') { return 2; }
	}
	return 0;
}

void login::OnBnClickedSignin()
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
	MainDialog client_GUI;
	client_GUI.DoModal();
}
