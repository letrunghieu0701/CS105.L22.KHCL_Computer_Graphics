
// MidPointView.cpp : implementation of the CMidPointView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MidPoint.h"
#endif

#include "MidPointDoc.h"
#include "MidPointView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMidPointView

IMPLEMENT_DYNCREATE(CMidPointView, CView)

BEGIN_MESSAGE_MAP(CMidPointView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMidPointView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMidPointView construction/destruction

CMidPointView::CMidPointView() noexcept
{
	// TODO: add construction code here

}

CMidPointView::~CMidPointView()
{
}

BOOL CMidPointView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMidPointView drawing

void Paint8Pixel(CDC* pDC, int x, int y, int x_center, int y_center, COLORREF circel_color)
{
	pDC->SetPixel(x + x_center, y + y_center, circel_color);
	pDC->SetPixel(-x + x_center, y + y_center, circel_color);

	pDC->SetPixel(x + x_center, -y + y_center, circel_color);
	pDC->SetPixel(-x + x_center, -y + y_center, circel_color);

	pDC->SetPixel(y + x_center, x + y_center, circel_color);
	pDC->SetPixel(-y + x_center, x + y_center, circel_color);

	pDC->SetPixel(y + x_center, -x + y_center, circel_color);
	pDC->SetPixel(-y + x_center, -x + y_center, circel_color);
}

void DrawCircelWithMidPoint(CDC* pDC, int x_center, int y_center, int r, COLORREF circel_color)
{
	int x = 0;
	int y = r;

	int p = 1 - r;
	while (x < y)
	{
		Paint8Pixel(pDC, x, y, x_center, y_center, circel_color);

		if (p < 0)
		{
			p = p + (x << 1) + 3;

			x = x + 1;
			// y = y;
		}
		else
		{
			p = p + 5 + ((x - y) << 1);

			x = x + 1;
			y = y - 1;
		}
	}
}

void CMidPointView::OnDraw(CDC *pDC)
{
	CMidPointDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	COLORREF circel_color = RGB(255, 0, 0);
	int x = 300;
	int y = 200;
	int r = 250;
	DrawCircelWithMidPoint(pDC, x, y, r, circel_color);
}


// CMidPointView printing


void CMidPointView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMidPointView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMidPointView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMidPointView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMidPointView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMidPointView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMidPointView diagnostics

#ifdef _DEBUG
void CMidPointView::AssertValid() const
{
	CView::AssertValid();
}

void CMidPointView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMidPointDoc* CMidPointView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMidPointDoc)));
	return (CMidPointDoc*)m_pDocument;
}
#endif //_DEBUG


// CMidPointView message handlers
