// newchh375.h : main header file for the NEWCHH375 application
//

#if !defined(AFX_NEWCHH375_H__2F4B3A2D_79E7_49EA_A188_983D3B3BEA35__INCLUDED_)
#define AFX_NEWCHH375_H__2F4B3A2D_79E7_49EA_A188_983D3B3BEA35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNewchh375App:
// See newchh375.cpp for the implementation of this class
//

class CNewchh375App : public CWinApp
{
public:
	CNewchh375App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewchh375App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNewchh375App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCHH375_H__2F4B3A2D_79E7_49EA_A188_983D3B3BEA35__INCLUDED_)
