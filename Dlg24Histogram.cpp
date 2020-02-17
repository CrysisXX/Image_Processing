// Dlg24Histogram.cpp : implementation file
//

#include "stdafx.h"
#include "newchh375.h"
#include "Dlg24Histogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg24Histogram dialog


CDlg24Histogram::CDlg24Histogram(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg24Histogram::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg24Histogram)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg24Histogram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg24Histogram)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg24Histogram, CDialog)
	//{{AFX_MSG_MAP(CDlg24Histogram)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg24Histogram message handlers
// 三色直方图导入
extern DWORD R[256];
extern DWORD G[256];
extern DWORD B[256];
void Color_Histogram();

BOOL CDlg24Histogram::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    Color_Histogram();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlg24Histogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	CDC *pDC=GetDC();
	
	int i, j, k;
	DWORD max1 = 0;
	DWORD max2 = 0;
	DWORD max3 = 0;

	//绘制方框--确定两个点(20,20)|(277,221)
	dc.Rectangle(39,40,297,242);
	
	//绘制方框
	dc.Rectangle(39,280,297,482);

	//绘制方框
	dc.Rectangle(39,520,297,722);
	
	//求出B数组中的最大值，归1化
	for(i = 0; i < 256; i++)
    {
		if(B[i] > max1)
		    max1 = B[i];
	}

	for(j = 0; j < 256; j++)
    {
		if(G[j] > max2)
		    max2 = G[j];
	}

	
	for(k = 0; k < 256; k++)
    {
		if(R[k] > max3)
		    max3 = R[k];
	}

	CPen pNewPen1;
    pNewPen1.CreatePen(PS_SOLID ,1,RGB(0,0,220)); //蓝色
	CPen* poldPen1=pDC->SelectObject(&pNewPen1);

	for(i = 0; i < 256; i++)
    {
		pDC->MoveTo(i + 40, 240);
		pDC->LineTo(i + 40, 240 - (int)(B[i] * 200 / max1));
	}
	pDC->SelectObject(poldPen1);

	CPen pNewPen2;
    pNewPen2.CreatePen(PS_SOLID ,1,RGB(0,220,0)); //绿色
	CPen* poldPen2=pDC->SelectObject(&pNewPen2);

	for(j = 0; j < 256; j++)
    {
		pDC->MoveTo(j + 40, 480);
		pDC->LineTo(j + 40, 480 - (int)(G[j] * 200 / max2));
	}
	pDC->SelectObject(poldPen2);

	CPen pNewPen3;
    pNewPen3.CreatePen(PS_SOLID ,1,RGB(220,0,0)); //红色
	CPen* poldPen3=pDC->SelectObject(&pNewPen3);

	for(k = 0; k < 256; k++)
    {
		pDC->MoveTo(k + 40, 720);
		pDC->LineTo(k + 40, 720 - (int)(R[k] * 200 / max3));
	}
	pDC->SelectObject(poldPen3);
}


