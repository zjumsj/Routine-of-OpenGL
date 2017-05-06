// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MAIN.h"


#include "MainFrm.h"

//

#include "BPnetwork.h"
#define PI 3.1415926535898
#define CI 7000
//70万次
//7000 1
//700000 0.01
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Global
	double outputdata[722];
	bool zero;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(RUN, OnRUN)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	zero=true;// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	if (!zero)
		delete bp;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnRUN() 
{
	int lay[3];
	double rate[3];
	double inputdata[18];

	int i;
	
	//各级神经元层数
	lay[0]=20;
	lay[1]=5;
	lay[2]=1;
	bp=new BPnetwork(3,lay);//BP
	//开始训练
	for (i=8;i>-1;i--){
		inputdata[i]=PI/4*i;
	}
	
	for (i=17;i>8;i--){
		//inputdata[i]=0.5+0.5*sin(PI/4*(i-1));//归一化
		inputdata[i]=0.5+0.25*sin(PI/4*(i-1));
	}
	
	
	bp->SetBPConfig(CI,0);
	rate[0]=1;//0.01
	rate[1]=1;
	rate[2]=1;
	//设置
	bp->BPTrain(inputdata,9,2,rate);
	
	//设置output
	for (i=0;i<361;i++)
		outputdata[i]=2*PI/360*i;
	
	//预测
	bp->BPBroadCast(outputdata,361);
	zero=false;
	Invalidate();
    MessageBox("Done!");	
}

