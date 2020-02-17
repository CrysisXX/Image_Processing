#if !defined(AFX_DLG24HISTOGRAM_H__B863F3FC_8A2A_4AA6_96E0_9B9FB1C5C2CF__INCLUDED_)
#define AFX_DLG24HISTOGRAM_H__B863F3FC_8A2A_4AA6_96E0_9B9FB1C5C2CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg24Histogram.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg24Histogram dialog

class CDlg24Histogram : public CDialog
{
// Construction
public:
	CDlg24Histogram(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg24Histogram)
	enum { IDD = IDD_24_HISTOGRAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg24Histogram)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg24Histogram)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG24HISTOGRAM_H__B863F3FC_8A2A_4AA6_96E0_9B9FB1C5C2CF__INCLUDED_)
