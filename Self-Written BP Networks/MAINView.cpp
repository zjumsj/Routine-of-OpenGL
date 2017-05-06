// MAINView.cpp : implementation of the CMAINView class
//

#include "stdafx.h"
#include "MAIN.h"

#include "MAINDoc.h"
#include "MAINView.h"

//
#include "math.h"
#define PI 3.14159265358978


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMAINView
//Global
extern double outputdata[722];
extern bool zero;

IMPLEMENT_DYNCREATE(CMAINView, CView)

BEGIN_MESSAGE_MAP(CMAINView, CView)
	//{{AFX_MSG_MAP(CMAINView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMAINView construction/destruction

CMAINView::CMAINView()
{
	// TODO: add construction code here

}

CMAINView::~CMAINView()
{
}

BOOL CMAINView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMAINView drawing

void CMAINView::OnDraw(CDC* pDC)
{
	CMAINDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect crect(200,100,922,700);
	CBrush cbrush(RGB(0,0,0));
	
	pDC->FrameRect(crect,&cbrush);
	CPen apen(PS_DASH,1,RGB(0,0,0));
	CPen * aold;
	aold=pDC->SelectObject(&apen);
	int i;
	for (i=150;i<700;i+=50){
		pDC->MoveTo(CPoint(200,i));
		pDC->LineTo(CPoint(922,i));
	}
	for (i=250;i<922;i+=50){
		pDC->MoveTo(CPoint(i,100));
		pDC->LineTo(CPoint(i,700));
	}
	pDC->SelectObject(aold);





	//
	if (!zero){
		double y[361];
		CPoint point[361];//声明CPoint类的一个对象point
		
		//定义画笔

		CPen cpen(PS_SOLID,2,RGB(120,0,0));
		CPen * pOldPen=pDC->SelectObject(&cpen);
		for (int i=0;i<361;i++)
		{
			point[i].x=200+2*i;
			y[i]=outputdata[361+i];
			point[i].y=400-200*4*(y[i]-0.5);//求出i点y的坐标值
		}
		for (i=0;i<360;i++){
			pDC->MoveTo(point[i]);
			pDC->LineTo(point[i+1]);
		}

		cpen.DeleteObject();
		cpen.CreatePen(PS_SOLID,2,RGB(0,120,0));
		pDC->SelectObject(&cpen);
		for (i=0;i<361;i++)
		{
			y[i]=sin(PI*2/360*i);
			point[i].y=400-200*y[i];
			
		}
		for (i=0;i<360;i++){
			pDC->MoveTo(point[i]);
			pDC->LineTo(point[i+1]);
		}
		pDC->SelectObject(pOldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMAINView printing

BOOL CMAINView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMAINView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMAINView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMAINView diagnostics

#ifdef _DEBUG
void CMAINView::AssertValid() const
{
	CView::AssertValid();
}

void CMAINView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMAINDoc* CMAINView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMAINDoc)));
	return (CMAINDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMAINView message handlers
