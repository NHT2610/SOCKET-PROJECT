#pragma once
#include "afxdialogex.h"


// login dialog

class login : public CDialogEx
{
	DECLARE_DYNAMIC(login)

public:
	login(CWnd* pParent = nullptr);   // standard constructor
	virtual ~login();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString username;
	CString password;
public:
	afx_msg void OnBnClickedSignup();
	afx_msg void OnBnClickedSignin();
	//Hàm kiểm tra định dạng username
	int checkUsername(CString& name);
	//Hàm kiểm tra định dạng password
	int checkPassword(CString& pass);
};
