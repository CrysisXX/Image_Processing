// newchh375View.h : interface of the CNewchh375View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWCHH375VIEW_H__AF2AC3A2_A754_4B6E_B55E_B799C35A2A53__INCLUDED_)
#define AFX_NEWCHH375VIEW_H__AF2AC3A2_A754_4B6E_B55E_B799C35A2A53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNewchh375View : public CScrollView
{
protected: // create from serialization only
	CNewchh375View();
	DECLARE_DYNCREATE(CNewchh375View)

// Attributes
public:
	CNewchh375Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewchh375View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewchh375View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNewchh375View)
	afx_msg void OnGray();
	afx_msg void OnGrayTrue();
	afx_msg void OnUpdateGrayTrue(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistogram();
	afx_msg void OnUpdateHistogram(CCmdUI* pCmdUI);
	afx_msg void OnLinetrans();
	afx_msg void OnUpdateLinetrans(CCmdUI* pCmdUI);
	afx_msg void OnEqualize();
	afx_msg void OnUpdateEqualize(CCmdUI* pCmdUI);
	afx_msg void OnFourier();
	afx_msg void OnUpdateFourier(CCmdUI* pCmdUI);
	afx_msg void OnIfourier();
	afx_msg void OnUpdateIfourier(CCmdUI* pCmdUI);
	afx_msg void OnFft();
	afx_msg void OnUpdateFft(CCmdUI* pCmdUI);
	afx_msg void OnIfft();
	afx_msg void OnUpdateIfft(CCmdUI* pCmdUI);
	afx_msg void OnClean();
	afx_msg void OnUpdateClean(CCmdUI* pCmdUI);
	afx_msg void OnAverageFilter();
	afx_msg void OnUpdateAverageFilter(CCmdUI* pCmdUI);
	afx_msg void OnMediumFilter();
	afx_msg void OnUpdateMediumFilter(CCmdUI* pCmdUI);
	afx_msg void OnLaplaceSharpening();
	afx_msg void OnUpdateLaplaceSharpening(CCmdUI* pCmdUI);
	afx_msg void OnGradSharpening();
	afx_msg void OnUpdateGradSharpening(CCmdUI* pCmdUI);
	afx_msg void OnPriwittsharpening();
	afx_msg void OnUpdatePriwittsharpening(CCmdUI* pCmdUI);
	afx_msg void OnGaussFilter();
	afx_msg void OnUpdateGaussFilter(CCmdUI* pCmdUI);
	afx_msg void OnButterworthLowFilter();
	afx_msg void OnUpdateButterworthLowFilter(CCmdUI* pCmdUI);
	afx_msg void OnIdealLowFilter();
	afx_msg void OnUpdateIdealLowFilter(CCmdUI* pCmdUI);
	afx_msg void OnIdealHighFilter();
	afx_msg void OnUpdateIdealHighFilter(CCmdUI* pCmdUI);
	afx_msg void OnButterworthHighFilter();
	afx_msg void OnUpdateButterworthHighFilter(CCmdUI* pCmdUI);
	afx_msg void On24histogram();
	afx_msg void OnUpdate24histogram(CCmdUI* pCmdUI);
	afx_msg void On24linetrans();
	afx_msg void OnUpdate24linetrans(CCmdUI* pCmdUI);
	afx_msg void On24equalize();
	afx_msg void OnUpdate24equalize(CCmdUI* pCmdUI);
	afx_msg void On24fft();
	afx_msg void OnUpdate24fft(CCmdUI* pCmdUI);
	afx_msg void On24ifft();
	afx_msg void OnUpdate24ifft(CCmdUI* pCmdUI);
	afx_msg void On24averageFilter();
	afx_msg void OnUpdate24averageFilter(CCmdUI* pCmdUI);
	afx_msg void On24mediumFilter();
	afx_msg void OnUpdate24mediumFilter(CCmdUI* pCmdUI);
	afx_msg void On24gaussFilter();
	afx_msg void OnUpdate24gaussFilter(CCmdUI* pCmdUI);
	afx_msg void On24laplaceSharpening();
	afx_msg void OnUpdate24laplaceSharpening(CCmdUI* pCmdUI);
	afx_msg void On24robertSharpening();
	afx_msg void OnUpdate24robertSharpening(CCmdUI* pCmdUI);
	afx_msg void On24priwittSharpening();
	afx_msg void OnUpdate24priwittSharpening(CCmdUI* pCmdUI);
	afx_msg void On24idealFilterLow();
	afx_msg void OnUpdate24idealFilterLow(CCmdUI* pCmdUI);
	afx_msg void On24idealFilterHigh();
	afx_msg void OnUpdate24idealFilterHigh(CCmdUI* pCmdUI);
	afx_msg void On24butterworthFilterLow();
	afx_msg void OnUpdate24butterworthFilterLow(CCmdUI* pCmdUI);
	afx_msg void On24butterworthFilterHigh();
	afx_msg void OnUpdate24butterworthFilterHigh(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in newchh375View.cpp
inline CNewchh375Doc* CNewchh375View::GetDocument()
   { return (CNewchh375Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCHH375VIEW_H__AF2AC3A2_A754_4B6E_B55E_B799C35A2A53__INCLUDED_)
