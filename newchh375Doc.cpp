// newchh375Doc.cpp : implementation of the CNewchh375Doc class
//

#include "stdafx.h"
#include "newchh375.h"

#include "newchh375Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewchh375Doc

IMPLEMENT_DYNCREATE(CNewchh375Doc, CDocument)

BEGIN_MESSAGE_MAP(CNewchh375Doc, CDocument)
	//{{AFX_MSG_MAP(CNewchh375Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewchh375Doc construction/destruction

CNewchh375Doc::CNewchh375Doc()
{
	// TODO: add one-time construction code here

}

CNewchh375Doc::~CNewchh375Doc()
{
}

BOOL CNewchh375Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNewchh375Doc serialization

void CNewchh375Doc::Serialize(CArchive& ar)
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
// CNewchh375Doc diagnostics

#ifdef _DEBUG
void CNewchh375Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNewchh375Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewchh375Doc commands

BOOL LoadBmpFile(char* BmpFileName);//在这里调用了LoadBmpFile
BOOL CNewchh375Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	LoadBmpFile((char *)lpszPathName);
	return TRUE;
}
