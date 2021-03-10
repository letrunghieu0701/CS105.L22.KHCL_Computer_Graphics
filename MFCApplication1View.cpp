
// MFCApplication1View.cpp : implementation of the CMFCApplication1View class
//

#include "pch.h"
#include "framework.h"
#include <utility>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCApplication1View construction/destruction

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: add construction code here

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View drawing


void DrawWithDDA(CDC* pDC,const POINT &start, const POINT &end, COLORREF line_color)
{
	// Nếu 2 điểm trùng nhau thì tô một trong 2 điểm rồi return
	if ((start.x == end.x) && (start.y == end.y))
	{
		pDC->SetPixel(start, line_color);
		return;
	}
	else
	{
		float x = start.x;
		float y = start.y;

		float dx = end.x - start.x;
		float dy = end.y - start.y;

		int step = max(abs(dx), abs(dy));

		float x_increment = dx / step;
		float y_increment = dy / step;

		// Thay đổi vị trí pixel để tô màu bằng cách nhích từng chút một x và y theo x_increment và y_increment
		// Giá trị increment thuộc [- vô cùng; + vô cùng] để có thể vẽ đoạn thằng mà không cần quan tâm thứ tự hay vị trí của 2 pixel đầu và cuối đoạn thẳng
		for (int i = 1; i <= step; i++)
		{
			pDC->SetPixel(static_cast<int>(round(x)), static_cast<int>(round(y)), line_color);
			x = x + x_increment;
			y = y + y_increment;
		}
	}
}

void DrawWithBresenham(CDC* pDC, POINT start, POINT end, COLORREF line_color)
{
	// Nếu 2 điểm trùng nhau thì tô một trong 2 điểm rồi return
	if ((start.x == end.x) && (start.y == end.y))
	{
		pDC->SetPixel(start, line_color);
		return;
	}
	else
	{
		//Vì công thức trong thuật toán chỉ dùng x+1 và y+1
		// Nên là phải luôn bắt đầu từ điểm bên trái nếu lặp theo x và phải luôn bắt đầu từ điểm bên dưới nếu lặp theo y
		
		/*if giảm chậm or tăng chậm
			Lặp theo dx*/
		if (abs(start.y - end.y) < abs(start.x - end.x))
		{
			// Swap 2 điểm để điểm bắt đầu luôn nằm bên trái
			if (start.x > end.x)
			{
				std::swap(start, end);
			}

			int x = start.x;
			int y = start.y;

			int dx = end.x - start.x;
			int dy = end.y - start.y;

			int y_step = (dy >= 0) ? 1 : -1;

			dx = abs(dx);
			dy = abs(dy);

			int P = 2 * dy - dx;
			int const_2dy = dy << 1;
			int const_2dy_subtract_2dx = (dy - dx) << 1;

			for (int i = 0; i <= dx; i++)
			{
				pDC->SetPixel(x, y, line_color);
				x++;

				if (P < 0)
				{
					P = P + const_2dy;
				}
				else
				{
					P = P + const_2dy_subtract_2dx;
					y += y_step;
				}
			}
		}
		/*else if giảm nhanh or tăng nhanh
			Lặp theo dy*/
		else
		{
			// Swap 2 điểm để điểm bắt đầu luôn nằm bên dưới
			if (start.y > end.y)
			{
				std::swap(start, end);
			}

			int x = start.x;
			int y = start.y;

			int dx = end.x - start.x;
			int dy = end.y - start.y;

			int x_step = (dx >= 0) ? 1 : -1;

			dx = abs(dx);
			dy = abs(dy);

			int P = 2 * dx - dy;
			int const_2dx = dx << 1;
			int const_2dx_subtract_2dy = (dx - dy) << 1;

			for (int i = 0; i <= dy; i++)
			{
				pDC->SetPixel(x, y, line_color);
				y++;

				if (P < 0)
				{
					P = P + const_2dx;
				}
				else
				{
					P = P + const_2dx_subtract_2dy;
					x += x_step;
				}
			}
		}
	}
}

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	COLORREF lineColor = RGB(255, 0, 0);
	POINT start;
	POINT end;


	// Đoạn thẳng đứng: DDA OK, Bresenham OK
	/*start.x = 100;
	start.y = 100;

	end.x = 100;
	end.y = 450;*/

	// Đoạn thẳng nằm ngang: DDA OK, Bresenham OK
	/*start.x = 100;
	start.y = 200;

	end.x = 500;
	end.y = 200;*/

	// Đoạn thẳng giảm nhanh: DDA OK, Bresenham OK
	/*start.x = 100;
	start.y = 450;

	end.x = 250;
	end.y = 100;*/

	// Đoạn thẳng giảm chậm: DDA OK, Bresenham OK
	/*start.x = 100;
	start.y = 300;

	end.x = 500;
	end.y = 200;*/


	// Đoạn thẳng tăng nhanh: DDA OK, Bresenham OK
	/*start.x = 100;
	start.y = 100;

	end.x = 250;
	end.y = 450;*/

	// Đoạn thẳng tăng chậm: DDA OK, Bresenham OK
	start.x = 100;
	start.y = 100;

	end.x = 500;
	end.y = 200;


	DrawWithDDA(pDC, start, end, lineColor);
	//DrawWithBresenham(pDC, start, end, lineColor);
	// 
	//DrawWithDDA(pDC, end, start, lineColor);
	//DrawWithBresenham(pDC, end, start, lineColor);

}


// CMFCApplication1View printing


void CMFCApplication1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication1View diagnostics

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View message handlers
