// newchh375View.cpp : implementation of the CNewchh375View class
//

#include "stdafx.h"
#include "newchh375.h"

#include "newchh375Doc.h"
#include "newchh375View.h"

//�Ի���ͷ�ļ�
#include "DlgHistogram.h"
#include "Dlg24Histogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewchh375View

IMPLEMENT_DYNCREATE(CNewchh375View, CScrollView)

BEGIN_MESSAGE_MAP(CNewchh375View, CScrollView)
	//{{AFX_MSG_MAP(CNewchh375View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_COMMAND(ID_GRAY_TRUE, OnGrayTrue)
	ON_UPDATE_COMMAND_UI(ID_GRAY_TRUE, OnUpdateGrayTrue)
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM, OnUpdateHistogram)
	ON_COMMAND(ID_LINETRANS, OnLinetrans)
	ON_UPDATE_COMMAND_UI(ID_LINETRANS, OnUpdateLinetrans)
	ON_COMMAND(ID_EQUALIZE, OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, OnUpdateEqualize)
	ON_COMMAND(ID_FOURIER, OnFourier)
	ON_UPDATE_COMMAND_UI(ID_FOURIER, OnUpdateFourier)
	ON_COMMAND(ID_IFOURIER, OnIfourier)
	ON_UPDATE_COMMAND_UI(ID_IFOURIER, OnUpdateIfourier)
	ON_COMMAND(ID_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(ID_FFT, OnUpdateFft)
	ON_COMMAND(ID_IFFT, OnIfft)
	ON_UPDATE_COMMAND_UI(ID_IFFT, OnUpdateIfft)
	ON_COMMAND(ID_CLEAN, OnClean)
	ON_UPDATE_COMMAND_UI(ID_CLEAN, OnUpdateClean)
	ON_COMMAND(ID_AVERAGE_FILTER, OnAverageFilter)
	ON_UPDATE_COMMAND_UI(ID_AVERAGE_FILTER, OnUpdateAverageFilter)
	ON_COMMAND(ID_MEDIUM_FILTER, OnMediumFilter)
	ON_UPDATE_COMMAND_UI(ID_MEDIUM_FILTER, OnUpdateMediumFilter)
	ON_COMMAND(ID_LAPLACE_SHARPENING, OnLaplaceSharpening)
	ON_UPDATE_COMMAND_UI(ID_LAPLACE_SHARPENING, OnUpdateLaplaceSharpening)
	ON_COMMAND(ID_GRAD_SHARPENING, OnGradSharpening)
	ON_UPDATE_COMMAND_UI(ID_GRAD_SHARPENING, OnUpdateGradSharpening)
	ON_COMMAND(ID_PRIWITTSHARPENING, OnPriwittsharpening)
	ON_UPDATE_COMMAND_UI(ID_PRIWITTSHARPENING, OnUpdatePriwittsharpening)
	ON_COMMAND(ID_GAUSS_FILTER, OnGaussFilter)
	ON_UPDATE_COMMAND_UI(ID_GAUSS_FILTER, OnUpdateGaussFilter)
	ON_COMMAND(ID_BUTTERWORTH_LOW_FILTER, OnButterworthLowFilter)
	ON_UPDATE_COMMAND_UI(ID_BUTTERWORTH_LOW_FILTER, OnUpdateButterworthLowFilter)
	ON_COMMAND(ID_IDEAL_LOW_FILTER, OnIdealLowFilter)
	ON_UPDATE_COMMAND_UI(ID_IDEAL_LOW_FILTER, OnUpdateIdealLowFilter)
	ON_COMMAND(ID_IDEAL_HIGH_FILTER, OnIdealHighFilter)
	ON_UPDATE_COMMAND_UI(ID_IDEAL_HIGH_FILTER, OnUpdateIdealHighFilter)
	ON_COMMAND(ID_BUTTERWORTH_HIGH_FILTER, OnButterworthHighFilter)
	ON_UPDATE_COMMAND_UI(ID_BUTTERWORTH_HIGH_FILTER, OnUpdateButterworthHighFilter)
	ON_COMMAND(ID_24HISTOGRAM, On24histogram)
	ON_UPDATE_COMMAND_UI(ID_24HISTOGRAM, OnUpdate24histogram)
	ON_COMMAND(ID_24LINETRANS, On24linetrans)
	ON_UPDATE_COMMAND_UI(ID_24LINETRANS, OnUpdate24linetrans)
	ON_COMMAND(ID_24EQUALIZE, On24equalize)
	ON_UPDATE_COMMAND_UI(ID_24EQUALIZE, OnUpdate24equalize)
	ON_COMMAND(ID_24FFT, On24fft)
	ON_UPDATE_COMMAND_UI(ID_24FFT, OnUpdate24fft)
	ON_COMMAND(ID_24IFFT, On24ifft)
	ON_UPDATE_COMMAND_UI(ID_24IFFT, OnUpdate24ifft)
	ON_COMMAND(ID_24AVERAGE_FILTER, On24averageFilter)
	ON_UPDATE_COMMAND_UI(ID_24AVERAGE_FILTER, OnUpdate24averageFilter)
	ON_COMMAND(ID_24MEDIUM_FILTER, On24mediumFilter)
	ON_UPDATE_COMMAND_UI(ID_24MEDIUM_FILTER, OnUpdate24mediumFilter)
	ON_COMMAND(ID_24GAUSS_FILTER, On24gaussFilter)
	ON_UPDATE_COMMAND_UI(ID_24GAUSS_FILTER, OnUpdate24gaussFilter)
	ON_COMMAND(ID_24LAPLACE_SHARPENING, On24laplaceSharpening)
	ON_UPDATE_COMMAND_UI(ID_24LAPLACE_SHARPENING, OnUpdate24laplaceSharpening)
	ON_COMMAND(ID_24ROBERT_SHARPENING, On24robertSharpening)
	ON_UPDATE_COMMAND_UI(ID_24ROBERT_SHARPENING, OnUpdate24robertSharpening)
	ON_COMMAND(ID_24PRIWITT_SHARPENING, On24priwittSharpening)
	ON_UPDATE_COMMAND_UI(ID_24PRIWITT_SHARPENING, OnUpdate24priwittSharpening)
	ON_COMMAND(ID_24IDEAL_FILTER_LOW, On24idealFilterLow)
	ON_UPDATE_COMMAND_UI(ID_24IDEAL_FILTER_LOW, OnUpdate24idealFilterLow)
	ON_COMMAND(ID_24IDEAL_FILTER_HIGH, On24idealFilterHigh)
	ON_UPDATE_COMMAND_UI(ID_24IDEAL_FILTER_HIGH, OnUpdate24idealFilterHigh)
	ON_COMMAND(ID_24BUTTERWORTH_FILTER_LOW, On24butterworthFilterLow)
	ON_UPDATE_COMMAND_UI(ID_24BUTTERWORTH_FILTER_LOW, OnUpdate24butterworthFilterLow)
	ON_COMMAND(ID_24BUTTERWORTH_FILTER_HIGH, On24butterworthFilterHigh)
	ON_UPDATE_COMMAND_UI(ID_24BUTTERWORTH_FILTER_HIGH, OnUpdate24butterworthFilterHigh)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewchh375View construction/destruction

CNewchh375View::CNewchh375View()
{
	// TODO: add construction code here

}

CNewchh375View::~CNewchh375View()
{
}

BOOL CNewchh375View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNewchh375View drawing

//����ȫ������
extern BITMAPINFO* lpBitsInfo;
extern BITMAPINFO* lpDIB_FFT;
extern BITMAPINFO* lpDIB_IFFT;
extern BITMAPINFO* new_lpBitsInfo;
//��ʾ����ͼ��
void CNewchh375View::OnDraw(CDC* pDC)
{
	CNewchh375Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	// ͼ�����ʧ�ܻ�δ���أ�ֱ�ӷ���
	if(NULL == lpBitsInfo)
	    return;
	

	RGBQUAD pal[2];
	pal[0].rgbRed = 255;
	pal[0].rgbGreen = 0;
	pal[0].rgbBlue = 0;
	pal[0].rgbReserved = 0;

	pal[1].rgbRed = 0;
	pal[1].rgbGreen = 255;
	pal[1].rgbBlue = 255;
	pal[1].rgbReserved = 0;

	//memcpy(lpBitsInfo->bmiColors, pal, 8);

	LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//ָ��lp2��λ��
	
	StretchDIBits(
		pDC->GetSafeHdc(),//����豸������
		//::GetDC(NULL),
		0,0,lpBitsInfo->bmiHeader.biWidth,lpBitsInfo->bmiHeader.biHeight,//��ʾ���С
		0,0,lpBitsInfo->bmiHeader.biWidth,lpBitsInfo->bmiHeader.biHeight,//ԭͼ���С���ı�ͼ����ʾ����biWidth/2��Height/2����ʾ���½��ķ�֮һ
		lpBits,//bitmap bits
		lpBitsInfo,//bitmap data
		DIB_RGB_COLORS,//usage options
		SRCCOPY//raster operation code
		//SRCINVERT //ȡ��
		);

	if(lpDIB_FFT)
	{
		lpBits = (LPVOID)&lpDIB_FFT->bmiColors[lpDIB_FFT->bmiHeader.biClrUsed];
		// ����
		StretchDIBits(
		pDC->GetSafeHdc(),//����豸������
		//::GetDC(NULL),
		600,0,lpDIB_FFT->bmiHeader.biWidth,lpDIB_FFT->bmiHeader.biHeight,//��ʾ���С
		0,0,lpDIB_FFT->bmiHeader.biWidth,lpDIB_FFT->bmiHeader.biHeight,//ԭͼ���С���ı�ͼ����ʾ����biWidth/2��Height/2����ʾ���½��ķ�֮һ
		lpBits,//bitmap bits
		lpDIB_FFT,//bitmap data
		DIB_RGB_COLORS,//usage options
		SRCCOPY//raster operation code
		//SRCINVERT //ȡ��
		);
	}

	if(lpDIB_IFFT)
	{
		lpBits = (LPVOID)&lpDIB_IFFT->bmiColors[lpDIB_IFFT->bmiHeader.biClrUsed];
		// ����
		StretchDIBits(
		pDC->GetSafeHdc(),//����豸������
		//::GetDC(NULL),
		0,600,lpDIB_IFFT->bmiHeader.biWidth,lpDIB_IFFT->bmiHeader.biHeight,//��ʾ���С
		0,0,lpDIB_IFFT->bmiHeader.biWidth,lpDIB_IFFT->bmiHeader.biHeight,//ԭͼ���С���ı�ͼ����ʾ����biWidth/2��Height/2����ʾ���½��ķ�֮һ
		lpBits,//bitmap bits
		lpDIB_IFFT,//bitmap data
		DIB_RGB_COLORS,//usage options
		SRCCOPY//raster operation code
		//SRCINVERT //ȡ��
		);
	}

	if(new_lpBitsInfo)
	{
		lpBits = (LPVOID)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];
		// ����
		StretchDIBits(
		pDC->GetSafeHdc(),//����豸������
		//::GetDC(NULL),
		600,0,new_lpBitsInfo->bmiHeader.biWidth,new_lpBitsInfo->bmiHeader.biHeight,//��ʾ���С
		0,0,new_lpBitsInfo->bmiHeader.biWidth,new_lpBitsInfo->bmiHeader.biHeight,//ԭͼ���С���ı�ͼ����ʾ����biWidth/2��Height/2����ʾ���½��ķ�֮һ
		lpBits,//bitmap bits
		new_lpBitsInfo,//bitmap data
		DIB_RGB_COLORS,//usage options
		SRCCOPY//raster operation code
		//SRCINVERT //ȡ��
		);
	}

}

void CNewchh375View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	// �ӳ�������
	sizeTotal.cx = sizeTotal.cy = 1500;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CNewchh375View printing

BOOL CNewchh375View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNewchh375View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNewchh375View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNewchh375View diagnostics

#ifdef _DEBUG
void CNewchh375View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CNewchh375View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CNewchh375Doc* CNewchh375View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewchh375Doc)));
	return (CNewchh375Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewchh375View message handlers

//α�Ҷ�
void gray_false();
void CNewchh375View::OnGray() 
{
	// TODO: Add your command handler code here
	gray_false();
	//���¿ͻ�����ͼ����ʾ����
	Invalidate();
}

//����G��ť�ļ�������
void CNewchh375View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    //�ǿ������24λ���ͼ��
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

//��Ҷ�
void gray_true();
void CNewchh375View::OnGrayTrue() 
{
	// TODO: Add your command handler code here
	gray_true();
    Invalidate();
	
}

//����G+��ť�ļ�������
void CNewchh375View::OnUpdateGrayTrue(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//�ǿ������24λ���ͼ��
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
	
}

//���������ʾ��ɫֵ���ߵ�����ص�ɫ��
void pixel(int i, int j, char* rgb);
void CNewchh375View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//pixel(point.y,point.x);
	CScrollView::OnLButtonDown(nFlags, point);
}


//��������ƶ����꣬�������귵����ɫֵ
void CNewchh375View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	

	//��¼����ֵ
	char xy[100];
	memset(xy,0,100);
	sprintf(xy,"x:%d y:%d     ",point.x,point.y);

	//��¼��ɫֵ
	char rgb[255];
	memset(rgb,0,255);
	pixel(point.x, point.y, rgb);

	//�ϲ�����ֵ����ɫֵ
	strcat(xy,rgb);
	
	//��״̬������ʾ
    ((CFrameWnd*)GetParent())->SetMessageText(xy);

    CScrollView::OnMouseMove(nFlags, point);
}

//��ʾ�Ҷ�ֱ��ͼ
void CNewchh375View::OnHistogram() 
{
	// TODO: Add your command handler code here
	CDlgHistogram dlg;
	dlg.DoModal();
}

//���ð�ťH�ļ�������
bool IsGray();
void CNewchh375View::OnUpdateHistogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
	//pCmdUI->Enable(lpBitsInfo != NULL);
}

void LineTrans();
void CNewchh375View::OnLinetrans() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	LineTrans();
	Invalidate();
}

void CNewchh375View::OnUpdateLinetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

void Equalize();
void CNewchh375View::OnEqualize() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	Equalize();
	Invalidate();
}

void CNewchh375View::OnUpdateEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

void Fourier();
void CNewchh375View::OnFourier() 
{
	// TODO: Add your command handler code here
	Fourier();
	Invalidate();
}

bool FD_exist();
bool Fast_FD_exist();
void CNewchh375View::OnUpdateFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && !FD_exist());
}

void IFourier();
void CNewchh375View::OnIfourier() 
{
	// TODO: Add your command handler code here
	IFourier();
    Invalidate();	
}

void CNewchh375View::OnUpdateIfourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && FD_exist());
	//Invalidate();
}

void Fast_Fourier();
void CNewchh375View::OnFft() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	if(new_lpBitsInfo)
	{
		free(new_lpBitsInfo);
		new_lpBitsInfo = NULL;
	}

	Fast_Fourier();
	Invalidate();
}

void CNewchh375View::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && !Fast_FD_exist());
}

void IFast_Fourier();
void CNewchh375View::OnIfft() 
{
	// TODO: Add your command handler code here
	IFast_Fourier();
	Invalidate();
}

void CNewchh375View::OnUpdateIfft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && Fast_FD_exist());
}

void Clear();
void CNewchh375View::OnClean() 
{
	// TODO: Add your command handler code here
	Clear();
	Invalidate();
}

void CNewchh375View::OnUpdateClean(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL || lpDIB_FFT != NULL || lpDIB_IFFT != NULL);
}

// ��ֵ�˲�
void AverageFilter();
void CNewchh375View::OnAverageFilter() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	AverageFilter();
	Invalidate();
}

void CNewchh375View::OnUpdateAverageFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

// ��ֵ�˲�
void MediumFilter();
void CNewchh375View::OnMediumFilter() 
{
	// TODO: Add your command handler code here
    if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	MediumFilter();
    Invalidate();
}

void CNewchh375View::OnUpdateMediumFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

// ��˹�˲�
void GaussFilter();
void CNewchh375View::OnGaussFilter() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	GaussFilter();
    Invalidate();
}

void CNewchh375View::OnUpdateGaussFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

// ������˹��
void LaplaceSharpening();
void CNewchh375View::OnLaplaceSharpening() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	LaplaceSharpening();
	Invalidate();
}

void CNewchh375View::OnUpdateLaplaceSharpening(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

// �ݶ���
void GradSharpening();
void CNewchh375View::OnGradSharpening() 
{
	// TODO: Add your command handler code here
		if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	GradSharpening();
	Invalidate();
}

void CNewchh375View::OnUpdateGradSharpening(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

//Priwitt��
void PriwittSharpening();
void CNewchh375View::OnPriwittsharpening() 
{
	// TODO: Add your command handler code here
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = NULL;
	}
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
    PriwittSharpening();
	Invalidate();
}

void CNewchh375View::OnUpdatePriwittsharpening(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray());
}

// ����Ƶ���˲�����
void IdealFilter(int choose);
void CNewchh375View::OnIdealLowFilter() 
{
	// TODO: Add your command handler code here
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	IdealFilter(0);
	Invalidate();
}

void CNewchh375View::OnUpdateIdealLowFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && Fast_FD_exist());
}

void CNewchh375View::OnIdealHighFilter() 
{
	// TODO: Add your command handler code here
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	IdealFilter(1);
	Invalidate();
}

void CNewchh375View::OnUpdateIdealHighFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && Fast_FD_exist());
}


// ������˹Ƶ���˲�����
void ButterworthFilter(int choose);
void CNewchh375View::OnButterworthLowFilter() 
{
	// TODO: Add your command handler code here
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	ButterworthFilter(0);
	Invalidate();
}

void CNewchh375View::OnUpdateButterworthLowFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && Fast_FD_exist());
}



void CNewchh375View::OnButterworthHighFilter() 
{
	// TODO: Add your command handler code here
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}
	ButterworthFilter(1);
	Invalidate();
}

void CNewchh375View::OnUpdateButterworthHighFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 8 && IsGray() && Fast_FD_exist());
}

void CNewchh375View::On24histogram() 
{
	// TODO: Add your command handler code here
	CDlg24Histogram dlg;
	dlg.DoModal();
}

void CNewchh375View::OnUpdate24histogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

//24λ���ͼ�������
//Slopeб�� Intercept�ؾ�
void Color_LineTrans(double Slope, double Intercept);
void CNewchh375View::On24linetrans() 
{
	// TODO: Add your command handler code here
	Color_LineTrans(1.1, -20.0);
	Invalidate();
}

void CNewchh375View::OnUpdate24linetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

//24λ���ͼ����⻯
void Color_Equalize();
void CNewchh375View::On24equalize() 
{
	// TODO: Add your command handler code here
	Color_Equalize();
    Invalidate();
}

void CNewchh375View::OnUpdate24equalize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// �ж��Ƿ�����˸���Ҷ�任
bool Color_FD_exist();
// 24λ���ͼ����ٸ���Ҷ�任
void Color_Fast_Fourier();
void CNewchh375View::On24fft() 
{
	// TODO: Add your command handler code here
	Color_Fast_Fourier();
	Invalidate();
}

void CNewchh375View::OnUpdate24fft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24 && !Color_FD_exist());
}

// 24λ���ͼ����ٸ���Ҷ���任
void Color_IFast_Fourier();
void CNewchh375View::On24ifft() 
{
	// TODO: Add your command handler code here
	Color_IFast_Fourier();
	Invalidate();
}

void CNewchh375View::OnUpdate24ifft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24 && Color_FD_exist());
}

// 24λ���ͼ���ֵ�˲�
void Color_AverageFilter();
void CNewchh375View::On24averageFilter() 
{
	// TODO: Add your command handler code here
	Color_AverageFilter();
	Invalidate();
}

void CNewchh375View::OnUpdate24averageFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// 24λ���ͼ����ֵ�˲�
void Color_MediumFilter();
void CNewchh375View::On24mediumFilter() 
{
	// TODO: Add your command handler code here
	Color_MediumFilter();
	Invalidate();
}

void CNewchh375View::OnUpdate24mediumFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// 24λ���ͼ���˹ģ��
void Color_GaussFilter();
void CNewchh375View::On24gaussFilter()
{
	// TODO: Add your command handler code here
    Color_GaussFilter();
	Invalidate();
}

void CNewchh375View::OnUpdate24gaussFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// 24λ���ͼ��������˹��
void Color_LaplaceSharpening();
void CNewchh375View::On24laplaceSharpening() 
{
	// TODO: Add your command handler code here
	Color_LaplaceSharpening();
	Invalidate();
}

void CNewchh375View::OnUpdate24laplaceSharpening(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// 24λ���ͼ��
// �ݶ��񻯡���Robert��
void Color_GradSharpening();
void CNewchh375View::On24robertSharpening() 
{
	// TODO: Add your command handler code here
	Color_GradSharpening();
	Invalidate();
}

void CNewchh375View::OnUpdate24robertSharpening(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// 24λ���ͼ��
// �ݶ��񻯡���Priwitt��
void Color_PriwittSharpening();
void CNewchh375View::On24priwittSharpening() 
{
	// TODO: Add your command handler code here
	Color_PriwittSharpening();
	Invalidate();
}

void CNewchh375View::OnUpdate24priwittSharpening(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

// 24λ���ͼ��
//----------------����Ƶ���˲�����---------------//
//choose=0 ��ͨ 1 ��ͨ 
void Color_IdealFilter(int choose);
void CNewchh375View::On24idealFilterLow() 
{
	// TODO: Add your command handler code here
	Color_IdealFilter(0);
	Invalidate();
}

void CNewchh375View::OnUpdate24idealFilterLow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24 && Color_FD_exist());
}

void CNewchh375View::On24idealFilterHigh() 
{
	// TODO: Add your command handler code here
	Color_IdealFilter(1);
	Invalidate();
}

void CNewchh375View::OnUpdate24idealFilterHigh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24 && Color_FD_exist());
}


// 24λ���ͼ��
//-------------������˹Ƶ���˲�����--------------//
//choose=0 ��ͨ 1 ��ͨ, nΪ����
void Color_ButterworthFilter(int choose);
void CNewchh375View::On24butterworthFilterLow() 
{
	// TODO: Add your command handler code here
	Color_ButterworthFilter(0);
	Invalidate();
}

void CNewchh375View::OnUpdate24butterworthFilterLow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24 && Color_FD_exist());
}

void CNewchh375View::On24butterworthFilterHigh() 
{
	// TODO: Add your command handler code here
	Color_ButterworthFilter(1);
	Invalidate();
}

void CNewchh375View::OnUpdate24butterworthFilterHigh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24 && Color_FD_exist());
}
