// MAINView.h : interface of the CMAINView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINVIEW_H__C7D0A27A_3BAF_444A_A85C_08556DEC7BD7__INCLUDED_)
#define AFX_MAINVIEW_H__C7D0A27A_3BAF_444A_A85C_08556DEC7BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMAINView : public CView
{
protected: // create from serialization only
	CMAINView();
	DECLARE_DYNCREATE(CMAINView)

// Attributes
public:
	CMAINDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMAINView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMAINView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMAINView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MAINView.cpp
inline CMAINDoc* CMAINView::GetDocument()
   { return (CMAINDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEW_H__C7D0A27A_3BAF_444A_A85C_08556DEC7BD7__INCLUDED_)
