// newchh375Doc.h : interface of the CNewchh375Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWCHH375DOC_H__7421FD1C_C007_4736_92A3_5151E2E62911__INCLUDED_)
#define AFX_NEWCHH375DOC_H__7421FD1C_C007_4736_92A3_5151E2E62911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNewchh375Doc : public CDocument
{
protected: // create from serialization only
	CNewchh375Doc();
	DECLARE_DYNCREATE(CNewchh375Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewchh375Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewchh375Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNewchh375Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCHH375DOC_H__7421FD1C_C007_4736_92A3_5151E2E62911__INCLUDED_)
