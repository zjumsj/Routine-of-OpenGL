// GameDlg.h : header file
//

#if !defined(AFX_GAMEDLG_H__0D13C113_3A02_44AF_8178_377489039A42__INCLUDED_)
#define AFX_GAMEDLG_H__0D13C113_3A02_44AF_8178_377489039A42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Layer.h"
/////////////////////////////////////////////////////////////////////////////
// CGameDlg dialog

class CGameDlg : public CDialog
{
// Construction
public:
	CGameDlg(CWnd* pParent = NULL);	// standard constructor
	Layer layer;
	bool left,right,up,down;
	int State;//状态机

	int pre;//前一状态
	int _flag;//前一状态标识字
	bool _i;//标识 1 从小到大 0 从大到小
	int count;//计数器
	CPoint Pos;//人物位置

// Dialog Data
	//{{AFX_DATA(CGameDlg)
	enum { IDD = IDD_GAME_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEDLG_H__0D13C113_3A02_44AF_8178_377489039A42__INCLUDED_)
