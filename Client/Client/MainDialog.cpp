// MainDialog.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "MainDialog.h"
#include "ClientSocket.h"
#include <string>

using namespace std;
// MainDialog dialog

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, DateString(COleDateTime::GetCurrentTime())
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, GOLD_LIST, GoldInfo);
	DDX_Control(pDX, DATE, SearchDate);
	DDX_Control(pDX, GOLD_TYPE, Type);
	DDX_DateTimeCtrl(pDX, DATE, DateString);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &MainDialog::OnBnClickedButton1)
	ON_BN_CLICKED(SEARCH, &MainDialog::OnBnClickedSearch)
END_MESSAGE_MAP()


// MainDialog message handlers

BOOL MainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	//Cài đặc combo box cho phần loại vàng
	Type.AddString(_T("DEFAULT"));
	Type.AddString(_T("DOJI"));
	Type.AddString(_T("SJC"));
	Type.SetCurSel(0);

	//Cài đặt bảng hiển thị giá vàng
	GoldInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GoldInfo.InsertColumn(0, _T("Loại vàng"), LVCFMT_LEFT, 400);
	GoldInfo.InsertColumn(1, _T("Giá mua (đơn vị: 1000đ)"), LVCFMT_LEFT, 300);
	GoldInfo.InsertColumn(2, _T("Giá bán (đơn vị: 1000đ)"), LVCFMT_LEFT, 300);
	GoldInfo.InsertColumn(3, _T("Ngày"), LVCFMT_LEFT, 400);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void MainDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//Snippet 01: Get Client Coordinate of the Rectangle
		CRect ClinetRect;
		this->GetClientRect(&ClinetRect);

		//Snippet 02: Declare Device Contexts and bitmap
		CDC memoryDC;
		CPaintDC DialogDC(this);
		CBitmap tiledImage;

		//Snippet 03: Load Image to Memory
		memoryDC.CreateCompatibleDC(&DialogDC);
		tiledImage.LoadBitmap(IDB_BITMAP3);
		memoryDC.SelectObject(&tiledImage);

		//Snippet 04: Copy memory pixels to dialog surface
		DialogDC.BitBlt(0, 0, ClinetRect.Width(), ClinetRect.Height(), &memoryDC, 0, 0, SRCCOPY);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR MainDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void MainDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	client_socket.SendMessageW(_T("QUIT"));
	int select = MessageBox(L"Ban co muon thoat?", L"Question", MB_YESNO | MB_ICONQUESTION);
	if (select == IDYES) {
		client_socket.SendMessageW(_T("YES"));
		client_socket.close();
		exit(1);
	}
	else if (select == IDNO) {
		client_socket.SendMessageW(_T("NO"));
	}
}

//Search thông tin tỷ giá vàng
void MainDialog::OnBnClickedSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Lấy thông tin ngày tháng năm
	int Day = DateString.GetDay();
	int Month = DateString.GetMonth();
	int Year = DateString.GetYear();
	string d, m, y;
	string SearchDay;
	if (Day < 10) { d += "0" + to_string(Day); }
	else { d = to_string(Day); }
	if (Month < 10) { m += "0" + to_string(Month); }
	else { m = to_string(Month); }
	y = to_string(Year);
	SearchDay += d + m + y;
	//Lấy thông tin của loại vàng
	CString gold_t;
	int index = Type.GetCurSel();
	Type.GetLBText(index, gold_t);
	string SearchType = CStringA(gold_t);

	//Chuỗi yêu cầu được gửi đi gồm (ngày tháng năm) + loại vàng
	string mess = SearchDay + "|" + SearchType;
	
	//Gửi request giá vàng đến server
	client_socket.SendMessageW(_T("GET_INFORMATION"));
	vector<ClientSocket::GoldInformations> DataRecv = client_socket.GetDataFromServer(mess);

	if (DataRecv.empty()) {
		MessageBox(L"Không có dữ liệu ngày bạn yêu cầu!", L"Information", MB_OK | MB_ICONINFORMATION);
	}
	else {
		//In data nhận được ra bảng
		int size = int(DataRecv.size());
		for (int i = 0; i < size; ++i) {
			CString field1(DataRecv[i].LoaiVang.c_str());
			GoldInfo.InsertItem(i, field1);
			CString field2(DataRecv[i].GiaMua.c_str());
			GoldInfo.SetItemText(i, 1, field2);
			CString field3(DataRecv[i].GiaBan.c_str());
			GoldInfo.SetItemText(i, 2, field3);
			CString field4(DataRecv[i].Ngay.c_str());
			GoldInfo.SetItemText(i, 3, field4);
		}
	}
	UpdateData(FALSE);
}
