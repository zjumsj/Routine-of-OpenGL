// MAIN.h : main header file for the MAIN application
//

#if !defined(AFX_MAIN_H__F58DE3E2_BEC3_4435_92BC_DA07A0212C25__INCLUDED_)
#define AFX_MAIN_H__F58DE3E2_BEC3_4435_92BC_DA07A0212C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMAINApp:
// See MAIN.cpp for the implementation of this class
//

class CMAINApp : public CWinApp
{
public:
	CMAINApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMAINApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMAINApp)
	afx_msg void OnAppAbout();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAIN_H__F58DE3E2_BEC3_4435_92BC_DA07A0212C25__INCLUDED_)
