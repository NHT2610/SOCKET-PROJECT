
// Client_GUIView.cpp : implementation of the CClientGUIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Client_GUI.h"
#endif

#include "Client_GUIDoc.h"
#include "Client_GUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientGUIView

IMPLEMENT_DYNCREATE(CClientGUIView, CView)

BEGIN_MESSAGE_MAP(CClientGUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CClientGUIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CClientGUIView construction/destruction

CClientGUIView::CClientGUIView() noexcept
{
	// TODO: add construction code here

}

CClientGUIView::~CClientGUIView()
{
}

BOOL CClientGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CClientGUIView drawing

void CClientGUIView::OnDraw(CDC* /*pDC*/)
{
	CClientGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CClientGUIView printing


void CClientGUIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CClientGUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CClientGUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CClientGUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CClientGUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CClientGUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CClientGUIView diagnostics

#ifdef _DEBUG
void CClientGUIView::AssertValid() const
{
	CView::AssertValid();
}

void CClientGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClientGUIDoc* CClientGUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientGUIDoc)));
	return (CClientGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CClientGUIView message handlers
