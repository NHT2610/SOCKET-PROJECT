// MainDialog.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "MainDialog.h"


// MainDialog dialog

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
END_MESSAGE_MAP()


// MainDialog message handlers
