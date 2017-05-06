// MAINDoc.cpp : implementation of the CMAINDoc class
//

#include "stdafx.h"
#include "MAIN.h"

#include "MAINDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMAINDoc

IMPLEMENT_DYNCREATE(CMAINDoc, CDocument)

BEGIN_MESSAGE_MAP(CMAINDoc, CDocument)
	//{{AFX_MSG_MAP(CMAINDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMAINDoc construction/destruction

CMAINDoc::CMAINDoc()
{
	// TODO: add one-time construction code here

}

CMAINDoc::~CMAINDoc()
{
}

BOOL CMAINDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMAINDoc serialization

void CMAINDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMAINDoc diagnostics

#ifdef _DEBUG
void CMAINDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMAINDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMAINDoc commands
