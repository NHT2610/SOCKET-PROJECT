
// ServerDlg.h : header file
//

#pragma once
#include "ServerManager.h"

class ServerManager;
using namespace std;

// CServerDlg dialog
class CServerDlg : public CDialogEx
{
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit ServerActivity;
	//afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedStart();
	ServerManager* m_pServerSock[5];
	void AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText);
	void ShowServerInfo(string sValue);
	afx_msg void OnBnClickedStopserver();
};
