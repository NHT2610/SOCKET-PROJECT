
// ClientDlg.h : header file
//

#pragma once


// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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
	//Hàm kiểm tra việc nhập IP
	bool checkIP(CString& ip);
private:
	CIPAddressCtrl IP_server;
public:
	afx_msg void OnIpnFieldchangedServer(NMHDR* pNMHDR, LRESULT* pResult);
	//Hàm click vào nút KẾT NỐI
	afx_msg void OnBnClickedBuuton();
	//Hàm click vào nút THOÁT
	afx_msg void OnBnClickedExit1();
};
