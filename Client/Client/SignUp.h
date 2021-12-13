#pragma once
#include "afxdialogex.h"


// SignUp dialog

class SignUp : public CDialogEx
{
	DECLARE_DYNAMIC(SignUp)

public:
	SignUp(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SignUp();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
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
	CString SignupUsername;
	CString SignupPassword;
	CString RePassword;
public:
	afx_msg void OnBnClickedSignup2();
	//Hàm kiểm tra định dạng username
	int checkUsername(CString& name);
	//Hàm kiểm tra định dạng password
	int checkPassword(CString& pass);
	//Hàm kiểm tra việc nhập lại password
	bool checkRePass(CString& pass, CString& repass);
};
