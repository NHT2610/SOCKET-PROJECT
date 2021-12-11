#pragma once
#include "afxdialogex.h"
#include <afxsock.h>

// Login dialog

class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	Login(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Login();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString username;
	CString password;
public:
	
	//Hàm kiểm tra định dạng username
	int checkUsername(CString& name);
	//Hàm kiểm tra định dạng password
	int checkPassword(CString& pass);
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedSignup1();
};
