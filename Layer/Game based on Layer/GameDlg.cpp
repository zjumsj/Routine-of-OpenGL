// GameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Game.h"
#include "GameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/*
----------------State Table-------------
 0a       1b        2c
 下       下        下
 3d       4e        5f
 左       左        左
 6g       7h        8i
 右       右        右
 9j      10k       11l
 上       上        上

状态字定义
0 下 1 左 2 右 3 上 静止
4 下 5 左 6 右 7 上 运动
*/
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDlg dialog

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	//Initialize
	left=false;
	right=false;
	up=false;
	down=false;
	
	State=0;//状态机状态0
	pre=1;//2号状态 朝下
	_flag=0;//静止朝下
	_i=false;

	count=0;//计数状态从零开始
	Pos.x=Pos.y=0;//设置起始位置在零零点

}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	//{{AFX_MSG_MAP(CGameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDlg message handlers

BOOL CGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	char c[2];
	char dir[14]="Pic\\Man\\a.bmp";
	int i;
	
	PicMat * picmat;
	//129 138
	layer.CreateBackGround(129,160,RGB(255,255,255));//建立黑色背景
	layer.Newlayer("BackGround");//新建一个背景
	picmat=layer.Current();
	picmat->LoadPic(CPoint(64,64),"Pic//Map//Map.bmp");//打开图像
	layer.Visible(false);//设置当前图层不可见
	c[1]=0;
	for (i=0;i<12;i++){//新建人物图层
		c[0]='a'+i;
		dir[8]=c[0];
		layer.Newlayer(c);
		layer.Visible(false);//设置为不可见
		picmat=layer.Current();//获取当前图层指针
		picmat->LoadPic(Pos,dir);//获得地图
		picmat->Transparent(RGB(255,255,255));//透明化
		picmat->Moveto(CPoint(64,64));//移动到中间位置
	}
	
	layer.Find("BackGround");
	layer.Copylayer("Map");//拷贝地图
	picmat=layer.Current();
	picmat->Cut(Pos,64,64);//图片切割
	layer.Visible(true);//设置为可见
	//显示Shadow
	layer.Newlayer("Shadow");
	picmat=layer.Current();
	picmat->LoadPic(CPoint(64,80),"Pic//Man//Shadow.bmp");//打开图像
	picmat->SetTransparent(170);
	picmat->Transparent(RGB(255,255,255));
	layer.Top();
	layer.Visible(true);
		
	//显示人物
	layer.Find("b");
	layer.Top();//置顶
	layer.Visible(true);//设置为可见

	
	//设置时间状态
	SetTimer(1,100,NULL);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CPaintDC dc(this);
		layer.Draw(&dc,(CDialog *)this);
			//这部分是Game需要加入的
//-------------------------------NOTICE------------------------------	
		CFont cf;
		CFont * cold;
		CString csx,csy;
		cf.CreateFont(   50,                        // nHeight
   20,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_BOLD,                 // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   _T("宋体"));                 // lpszFacename
		cold=dc.SelectObject(&cf);//获得字体
		csx.Format("%d",Pos.x);
		csy.Format("%d",Pos.y);
		
		dc.TextOut(20,380,CString("位置：X=")+csx+CString(" Y=")+csy);
		dc.SelectObject(cold);
//---------------------------------------------------------------- 


	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//上下左右键无用
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar==VK_DOWN)
		down=false;
	else if (nChar==VK_UP)
		up=false;
	else if (nChar==VK_LEFT)
		left=false;
	else if (nChar==VK_RIGHT)
		right=false;
	
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CGameDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	 if   (pMsg-> message   ==   WM_KEYDOWN)         //若 按键消息
     {
		 switch   (pMsg-> wParam)                 //获取按键虚键值
         {
			case VK_DOWN:down=true;break;
			case VK_UP: up=true; break;
			case VK_LEFT: left=true;break;
			case VK_RIGHT: right=true;break;
		 
         } 
    } 
        
	return CDialog::PreTranslateMessage(pMsg);
}

void CGameDlg::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}

void CGameDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//count++;
	//状态机判断
	int i;
	int this_pre;//本次的用图片
	int this_flag;//本次的状态
	PicMat * picmat;
	char c[2];
	c[1]=0;

	i=0;
	if (left)
		i++;
	if (right)
		i++;
	if (up)
		i++;
	if (down)
		i++;
	
	//i>1 保持上次的状态
	//i==1 进行这次的行动
	//i=0 如果运动就静止
	if (i==1){
		if(down){
			if (_flag==4){//继续运动
				this_flag=_flag; 
				if(_i){
					this_pre=pre+1;
					if(this_pre==2)
						_i=false;
				}
				else{
					this_pre=pre-1;
					if(this_pre==0)
						_i=true;
				}
			
			}
			else{//进入向下运动初始状态
				this_pre=0;
				this_flag=4;
				_i=true;
			}		
		}
		else if (left){
			if (_flag==5){//继续运动
				this_flag=5;
				if(_i){
					this_pre=pre+1;
					if(this_pre==5)
						_i=false;
				}
				else{
					this_pre=pre-1;
					if(this_pre==3)
						_i=true;
				}
			}
			else{//进入向左运动初始状态
				this_pre=3;
				this_flag=5;
				_i=true;
			}
		}
		else if (right){
			if(_flag==6){//继续运动
				this_flag=6;
				if(_i){
					this_pre=pre+1;
					if(this_pre==8)
						_i=false;
				}
				else{
					this_pre=pre-1;
					if(this_pre==6)
						_i=true;
				}
			}
			else{//进入向右运动初始状态
				this_pre=6;
				this_flag=6;
				_i=true;
			}
		}
		else{
			if(_flag==7){//继续活动
				this_flag=7;
				if(_i){
					this_pre=pre+1;
					if(this_pre==11)
						_i=false;
				}
				else{
					this_pre=pre-1;
					if(this_pre==9)
						_i=true;
				}
			}
			else{//进入向上运动初始状态
				this_pre=9;
				this_flag=7;
				_i=true;
			}
		}
	}
	else if (i==0){//进入静止状态
		if(_flag==4){
			this_flag=0;
			this_pre=1;
			_i=false;
		}
		else if (_flag==5){
			this_flag=1;
			this_pre=4;
			_i=false;
		}
		else if (_flag==6){
			this_flag=2;
			this_pre=7;
			_i=false;		
		}
		else if (_flag==7){
			this_flag=3;
			this_pre=10;
			_i=false;
		}
		else{//原本就静止
			this_flag=_flag;
			this_pre=pre;
			//_i状态保持不变
		}
	}
	else{//粘滞键
		this_flag=_flag;
		if(_flag<4){//静止环节
			this_pre=pre;
		}
		else{
			if(_i)
				this_pre=pre+1;
			else
				this_pre=pre-1;
			if(this_pre%3==2){//运动环节加判定
				_i=false;
			}
			else if(this_pre%3==0){
				_i=true;
			}
		}
	}
	//人物位移处理
	switch(this_flag){
	case 4: Pos.y-=3;break;
	case 5: Pos.x-=3;break;
	case 6: Pos.x+=3;break;
	case 7: Pos.y+=3;break;
	}
	//图像处理 地图处理
	layer.Find("Map");
	layer.Delete();
	layer.Find("BackGround");
	layer.Copylayer("Map");
	layer.Visible(true);
	picmat=layer.Current();
	picmat->Cut(Pos,64,64);//图片切割
	//图像处理 阴影处理
	layer.Find("Shadow");
	layer.Top();
	//图像处理 人物处理
	c[0]='a'+pre;
	layer.Find(c);
	layer.Visible(false);//停用之前的图层
	c[0]='a'+this_pre;
	layer.Find(c);
	layer.Visible(true);//启用本次的图层
	layer.Top();//置顶操作
		
	//重新赋值
	pre=this_pre;
	_flag=this_flag;
	
	Invalidate(false);

	CDialog::OnTimer(nIDEvent);
}
