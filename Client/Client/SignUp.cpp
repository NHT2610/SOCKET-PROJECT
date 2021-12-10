// SignUp.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "SignUp.h"


// SignUp dialog

IMPLEMENT_DYNAMIC(SignUp, CDialogEx)

SignUp::SignUp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

SignUp::~SignUp()
{
}

void SignUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SignUp, CDialogEx)
END_MESSAGE_MAP()


// SignUp message handlers
