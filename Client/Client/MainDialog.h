#pragma once
#include "afxdialogex.h"


// MainDialog dialog

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
