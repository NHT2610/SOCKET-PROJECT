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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
