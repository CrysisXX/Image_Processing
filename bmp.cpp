#include "stdafx.h"
#include<complex>

#define PI 3.1415962535
using namespace std;

BITMAPINFO* lpBitsInfo = NULL;
BITMAPINFO* lpDIB_FFT = NULL;
BITMAPINFO* lpDIB_IFFT = NULL;
BITMAPINFO* new_lpBitsInfo = NULL;

BOOL LoadBmpFile(char* BmpFileName)
{
	FILE* fp;
	if( NULL == (fp = fopen(BmpFileName,"rb")))
		return false;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	fread(&bf, 14, 1, fp);//�ļ�ͷ
	fread(&bi, 40, 1, fp);//��Ϣͷָ�룬ʵ��λͼ����������������

	//����ҪΪָ��lpBitsInfo�������size
	DWORD NumColors;//��ɫ��ʵ�ʳ���
	if(bi.biClrUsed != 0)
		NumColors = bi.biClrUsed;
	else
	{
		switch(bi.biBitCount)
		{
		case 1:
			NumColors = 2;
			break;
		case 4:
			NumColors = 16;
			break;
		case 8:
			NumColors = 256;
			break;
		case 24:
			NumColors = 0;//���ͼ��û�е�ɫ��
			break;
		}
	}

	DWORD LineBytes = (bi.biWidth * bi.biBitCount + 31) / 32 * 4;//ÿһ�е��ֽ���
	DWORD size = 40 + NumColors * 4 + LineBytes * bi.biHeight;//�ܴ�С = ��Ϣͷ���� + ��ɫ�峤�� * 4 + ͼ���С

	if(NULL == (lpBitsInfo = (BITMAPINFO*)malloc(size)))//ΪlpBitsInfo�����ڴ�ռ䣬��СΪsize
		return false;//�������ʧ�ܣ��򷵻�false

	fseek(fp, 14, SEEK_SET);//����ָ��lp1��λ��
    fread((char*)lpBitsInfo, size, 1, fp);//��������
	fclose(fp);


	//LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//ָ��2��λ��

	lpBitsInfo->bmiHeader.biClrUsed = NumColors;//����ͼƬ��ʹ�õ���ɫ����������ɫ���ʵ�ʳ���

	return true;
}

void gray_false()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	BYTE *R, *G, *B, avg;
	//BYTE avg;

	for(i = 0; i < h; i++)
	{
	    for(j = 0; j < w; j++)
		{
			B = lpBits + i * LineBytes + j * 3;
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B) / 3;
            *R = *G = *B = avg;
		}
	
	}
}

//24λ���ͼ��ת��Ϊ�Ҷ�ͼ��
void gray_true()
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int new_LineBytes = (w * 8 + 31) / 32 * 4; //��λ�Ҷ�ͼ���ÿһ�е��ֽ���
	LONG size = 40 + 1024 + new_LineBytes * h; //1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
	
	LPBITMAPINFO new_lpBitsInfo = (LPBITMAPINFO)malloc(size);//Ϊָ��1�����ڴ棬��СΪsize

	memcpy(new_lpBitsInfo, lpBitsInfo, 40); //������Ϣͷ
    new_lpBitsInfo->bmiHeader.biBitCount = 8; //�޸�ͼ��λ��
	new_lpBitsInfo->bmiHeader.biClrUsed = 256; //�޸�ͼ����ʹ�õ���ɫ��

	int i,j;
	for(i = 0; i < 256; i++) //����ɫ��
	{
		new_lpBitsInfo->bmiColors[i].rgbRed = i;
        new_lpBitsInfo->bmiColors[i].rgbGreen = i;
		new_lpBitsInfo->bmiColors[i].rgbBlue = i;
		new_lpBitsInfo->bmiColors[i].rgbReserved = i;
	}

	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];//�ض�λ��ָ��2��λ��
	BYTE *R, *G, *B, avg, *pixel;

	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			B = lpBits + (h - 1 - i) * LineBytes + j * 3;
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B) / 3;
            *R = *G = *B = avg;
			
			pixel = new_lpBits + new_LineBytes * (h - 1 - i) + j;
			*pixel = avg;
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;
}

//���ش���
bool IsGray()
{
	int judge_gray = 0;
    int first1,first2,first3;
    int mid1,mid2,mid3;
    int last1,last2,last3;
    first1 = first2 = first3 = 0;
    mid1 = mid2 = mid3 =0;
    last1 = last2 = last3 = 0;

	//�жϻҶ�ͼ��
	first1 = lpBitsInfo->bmiColors[0].rgbRed;
	first2 = lpBitsInfo->bmiColors[0].rgbGreen;
	first3 = lpBitsInfo->bmiColors[0].rgbBlue;
	if(first1 == first2 && first2 == first3)
		judge_gray++;

	mid1 = lpBitsInfo->bmiColors[127].rgbRed;
	mid2 = lpBitsInfo->bmiColors[127].rgbGreen;
	mid3 = lpBitsInfo->bmiColors[127].rgbBlue;
	if(mid1 == mid2 && mid2 == mid3)
		judge_gray++;

	last1 = lpBitsInfo->bmiColors[255].rgbRed;
	last2 = lpBitsInfo->bmiColors[255].rgbGreen;
	last3 = lpBitsInfo->bmiColors[255].rgbBlue;
	if(last1 == last2 && last2 == last3)
		judge_gray++;

	if(judge_gray == 3)
		return true;
	else 
		return false;
}

void pixel(int i, int j, char* str)
{

    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	if(i >= h || j >= w)
		return; //����������Χ����ͼ��Χ����ֱ�ӷ���


	if(i > h || j > w)// �������Χ����ͼ����ʾ���أ�����
		return;

	BYTE *pixel, bv;
	int r, g, b;
	int index = 0;//case 4������ֱ�Ӹ���*pixel��ֵ����Ҫ����һ���µ�ֵ���н���

	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
    case 24:
		pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;//(h-i-1)��ȡ������
		b = *pixel;
		g = *(pixel + 1);
		r = *(pixel + 2);
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		break;
	
	case 8:
		pixel = lpBits + LineBytes * (h - 1 - i) + j;

		if(!IsGray())
		{
			r = lpBitsInfo->bmiColors[*pixel].rgbRed; //����pixel��������8λͼ��ĵ�ɫ�壬�õ������RGB��ֵ
			g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
			sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		}
		else
			sprintf(str, "�Ҷ�ֵ��%d", *pixel);

		break;

	case 4:
		pixel = lpBits + LineBytes * (h - 1 - i) + j / 2 ;
		if(0 == j % 2)
			index = *pixel >> 4;//��������ż�������ҵ��Ǹ��ֽڵĸ���λ�������ƶ�4λ
		else
			index = *pixel & 15;//�����������������ҵ��Ǹ��ֽڵĵ���λ����00001111=15��������

	    r = lpBitsInfo->bmiColors[index].rgbRed; //����pixel��������4λͼ��ĵ�ɫ�壬�õ������RGB��ֵ
		g = lpBitsInfo->bmiColors[index].rgbGreen;
		b = lpBitsInfo->bmiColors[index].rgbBlue;
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);

		break;

	case 1:
		//j/8���ĸ��ֽ���
		//bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7  - j % 8));//�ڴ���ʽ������λ������λ 1,2,3,4|5,6,7,8//�պ��ܱ�8�����ǵ�һλ����7���������һλ 0 1 2 3 4 5 6 7 | 8 9 10...
		bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7  - j % 8));

		if(0 == bv)
		{
			//r = lpBitsInfo->bmiColors[0].rgbRed; //����pixel��������1λͼ��ĵ�ɫ�壬�õ������RGB��ֵ
			//g = lpBitsInfo->bmiColors[0].rgbGreen;
			//b = lpBitsInfo->bmiColors[0].rgbBlue;
			//sprintf(str, "����ɫRGB(%d,%d,%d)", r, g, b);
			sprintf(str, "����ɫ");
		}
		else
		{
			//r = lpBitsInfo->bmiColors[1].rgbRed; //����pixel��������1λͼ��ĵ�ɫ�壬�õ������RGB��ֵ
			//g = lpBitsInfo->bmiColors[1].rgbGreen;
			//b = lpBitsInfo->bmiColors[1].rgbBlue;
			sprintf(str, "ǰ��ɫ");
		}
		break;
	}
	//AfxMessageBox(str);
}

//���ȫ�ֻҶ�ֱ��ͼ��¼����
DWORD H[256];
void Histogram()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i,j;
	BYTE *pixel;

	//�Ҷ�ֱ��ͼ�����¼ֵ��ʼ��
	for(i = 0; i <256; i++)
	{
		H[i] = 0;
	}

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			//pixelָ��8λ�Ҷ�ͼ������ص㣬����ֵ��Ϊ�Ҷ�ֵ
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			H[*pixel]++;
        }
	}
}

// 24λ���ͼ��ֱ��ͼ
DWORD R[256];
DWORD G[256];
DWORD B[256];
void Color_Histogram()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i, j, r, g, b;
	BYTE *pixel;

	//��ɫֱ��ͼ�����¼ֵ��ʼ��
	for(i = 0; i <256; i++)
	{
		R[i] = 0;
		G[i] = 0;
		B[i] = 0;
	}

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			//pixelָ��24λ���ͼ��ͼ������ص�
			pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
			//��ɫ��ǰ����ɫ���У���ɫ���
			b = *pixel;
			g = *(pixel + 1);
		    r = *(pixel + 2);
			B[b]++;
			G[g]++;
			R[r]++;
        }
	}
}

//8λ�Ҷ�ͼ�������
void LineTrans()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	float a,b;
	float bv;
	BYTE *pixel;
	//�������߲���
	//б��
	a = 1.1f;
	//�ؾ�
	b = -20;
    for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			bv = *pixel * a + b;
			if(bv > 255)
				bv = 255;
			if(bv < 0)
				bv = 0;
			*pixel = (BYTE)(bv + 0.5); //��������
		}
	}

}

//24λ���ͼ�������
//Slopeб�� Intercept�ؾ�
void Color_LineTrans(double Slope, double Intercept)
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	double R_pixel, G_pixel, B_pixel;
	BYTE *pixel;
	//�������߲���

    for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
			B_pixel = *pixel * Slope + Intercept;
			G_pixel = *(pixel + 1) * Slope + Intercept;
			R_pixel = *(pixel + 2) * Slope + Intercept;

			// blue
			if(B_pixel > 255)
				B_pixel = 255;
			if(B_pixel < 0)
				B_pixel = 0;
			// green
			if(G_pixel > 255)
				G_pixel = 255;
			if(G_pixel < 0)
				G_pixel = 0;
			// red
            if(R_pixel > 255)
				R_pixel = 255;
			if(R_pixel < 0)
				R_pixel = 0;
            //+0.5��������
			*pixel = (BYTE)(B_pixel + 0.5); 
			*(pixel + 1) = (BYTE)(G_pixel + 0.5);
			*(pixel + 2) = (BYTE)(R_pixel + 0.5);
		}
	}
}


// 8λ�Ҷ�ͼ����⻯
void Equalize()
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	float temp;
	BYTE *pixel;
	//�Ҷ�ӳ���
	int Map[256];
	Histogram();
    
	//����Ҷ�ӳ���
    for(i = 0; i < 256; i++)
	{
		//ÿ��ͳ�ƶ�Ҫ��temp�����ʼֵ
		temp = 0;
		for(j = 0; j <= i; j++)
		{
            temp += H[j];
		}
		//��������
		Map[i] = (BYTE)((255 * temp) / (w * h) + 0.5);
	}

	//���⻯
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = Map[*pixel];
		}
	}
}

// 24λ���ͼ����⻯
void Color_Equalize()
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	float R_temp, G_temp, B_temp;
	BYTE *pixel;
	//�Ҷ�ӳ���
	int R_Map[256], G_Map[256], B_Map[256];
	Color_Histogram();
    
	//����Ҷ�ӳ���
    for(i = 0; i < 256; i++)
	{
		//ÿ��ͳ�ƶ�Ҫ��temp�����ʼֵ
		R_temp = 0;
		for(j = 0; j <= i; j++)
		{
            R_temp += R[j];
		}
		//��������
		R_Map[i] = (BYTE)((255 * R_temp) / (w * h) + 0.5);
	}

	for(i = 0; i < 256; i++)
	{
		//ÿ��ͳ�ƶ�Ҫ��temp�����ʼֵ
		G_temp = 0;
		for(j = 0; j <= i; j++)
		{
            G_temp += G[j];
		}
		//��������
		G_Map[i] = (BYTE)((255 * G_temp) / (w * h) + 0.5);
	}

	for(i = 0; i < 256; i++)
	{
		//ÿ��ͳ�ƶ�Ҫ��temp�����ʼֵ
		B_temp = 0;
		for(j = 0; j <= i; j++)
		{
            B_temp += B[j];
		}
		//��������
		B_Map[i] = (BYTE)((255 * B_temp) / (w * h) + 0.5);
	}

	//���⻯
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
			*pixel = B_Map[*pixel];
			*(pixel + 1) = G_Map[*(pixel + 1)];
			*(pixel + 2) = R_Map[*(pixel + 2)];
		}
	}
}

void FT(complex<double>* TD, complex<double>* FD, int M)
{
	int u, x;
	double angle;
	// M������Ҫ���и���Ҷ�任�ĵ���
	for(u = 0; u < M; u++)
	{
		// ��ʼ��FDΪ0
		FD[u] = 0;
		for(x = 0; x < M; x++)
		{
			angle = -2 * PI * u * x / M;
			// complex<double>(cos,sin)��ǰ����ʵ�����������鲿
			// ŷ����ʽ e^(i*angle) = cos(angle) + i * sin(angle)
			FD[u] += TD[x] * complex<double>(cos(angle), sin(angle)); 
		}
		FD[u] /= M;
	}
}

void IFT(complex<double>* TD, complex<double>* FD, int M)
{
	int u, x;
    double angle;
	// M������Ҫ���и���Ҷ���任�ĵ���
	for(x = 0; x < M; x++)
	{
		// ��ʼ��TDΪ0
		TD[x] = 0;
		for(u = 0; u < M; u++)
		{
			angle = 2 * PI * u * x / M;
			// complex<double>(cos,sin)��ǰ����ʵ�����������鲿
			// ŷ����ʽ e^(i*angle) = cos(angle) + i * sin(angle)
			TD[x] += FD[u] * complex<double>(cos(angle), sin(angle)); 
		}
	}
}

// ȫ�ֻ�ȡ���θ���Ҷ�任֮���FD
complex<double>* tempFD;
// ȫ�ֻ�ȡ���θ���Ҷ�任֮���FD
complex<double>* R_tempFD;
complex<double>* G_tempFD;
complex<double>* B_tempFD;

int Fourier_stage = 0;
int Fast_Fourier_stage = 0;

// �ж�tempFD�������
bool FD_exist()
{
	if(tempFD && Fourier_stage)
		return true;
	else
		return false;
}

// �ж�tempFD�������
bool Color_FD_exist()
{
	if(R_tempFD && G_tempFD && B_tempFD && Fast_Fourier_stage)
		return true;
	else
		return false;
}

void Fourier()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h]; //����ռ�
	complex<double>* FD = new complex<double>[w * h]; //Ƶ��ռ�

	int i,j;
	BYTE* pixel;
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			// ����ͼ��Ҷ�ֵ
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			// TD[w * i + j] = complex<double>(*pixel, 0);  //����Ҷ�ֵ
			// ƽ�Ʋ���
			TD[w * i + j] = complex<double>(*pixel * pow(-1, i + j), 0);
		}
	}

	for(i = 0; i < h; i++)
	{
		FT(&TD[w * i], &FD[w * i], w);  //��ˮƽ����ÿһ�ж�������Ҷ�任��һ����w����
	}

	//�ش�ֱ����������Ҷ�任ʱ����Ϊ���ص�ĻҶ�ֵ����������һ��С��������ת�ã���������TD���ڴ�ռ����洢ת�õ�TD
	//ת��
    for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			TD[h * j + i] = FD[w * i + j];
		}
	}

	for(i = 0; i < w; i++)
	{
		FT(&TD[h * i], &FD[h * i], h);  //�ش�ֱ����ÿһ�ж�������Ҷ�任��һ����h����
	}

	// ��ȡ���θ���Ҷ�任��Ƶ��ռ�
	tempFD = FD;

	long size = 40 + 1024 + LineBytes * h; // ��Ϣͷ+��ɫ�� // 1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
    BITMAPINFO* lpDIB_FT = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_FT�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_FT)
		return;

	memcpy(lpDIB_FT, lpBitsInfo, size);
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			temp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + FD[j * h + i].imag() * FD[j * h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}

	// �任��ɣ����и���Ҷ�任���
	Fourier_stage = 1;
	// �ͷ��ڴ�
	delete TD;
	// ��ԭͼ��������ʾ
	free(lpBitsInfo);
    lpBitsInfo = lpDIB_FT;
}

void IFourier()
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h]; //����ռ�
	complex<double>* FD = new complex<double>[w * h]; //Ƶ��ռ�

	int i,j;
	BYTE* pixel;

	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			FD[w * i + j] = tempFD[h * j + i];
		}
	}

	//��ˮƽ����ÿһ�ж�������Ҷ���任��һ����w����
	for(i = 0; i < h; i++)
	{
		IFT(&TD[w * i], &FD[w * i], w);  
	}

	//�ش�ֱ����������Ҷ���任ʱ����Ϊ���ص�ĻҶ�ֵ����������һ��С��������ת�ã���������TD���ڴ�ռ����洢ת�õ�TD
	//ת��
    for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			FD[h * j + i] = TD[w * i + j];
		}
	}

	 //�ش�ֱ����ÿһ�ж�������Ҷ���任��һ����h����
	for(i = 0; i < w; i++)
	{
		IFT(&TD[h * i], &FD[h * i], h); 
	}

	long size = 40 + 1024 + LineBytes * h; // ��Ϣͷ+��ɫ�� // 1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
    BITMAPINFO* lpDIB_IFT = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_IFT�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_IFT)
		return;

	memcpy(lpDIB_IFT, lpBitsInfo, 1064);
	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			temp = TD[j * h + i].real() / pow(-1, i + j);
			if(temp < 0)
				temp = 0;
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}

	// ���任��ɣ����и���Ҷ�任���
	Fourier_stage = 0;
	// �ͷ��ڴ�
	delete FD;
	delete TD;
	delete tempFD;
	free(lpBitsInfo);
    lpBitsInfo = lpDIB_IFT;
}

// �ж�tempFD�������
bool Fast_FD_exist()
{
	if(tempFD && Fast_Fourier_stage)
		return true;
	else
		return false;
}

// һά��ɢ���ٸ���Ҷ�任
// �����count������2����,�����r��ָ����Ҷ�任������ָ��
void FFT(complex<double>* TD, complex<double>* FD, int r)
{
	int i,j,k;
	double angle;
	// ������Ҫ����Ҷ�任�ĵ���
	long count = 1 << r;
	// �����ڴ�
	complex<double>* W = new complex<double>[count / 2];

	// �����Ȩϵ��
	for(i = 0; i < count / 2; i++)
	{
		angle = -2 * PI * i / count;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	// ÿһ������㷨�����������
	complex<double>* X1 = new complex<double>[count]; // ����
	complex<double>* X2 = new complex<double>[count]; // ���

	// ��ʱ��ռ�ĻҶ�ֵд��X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// �����㷨
	int layer, group_size, times, index;
	// ��ʱ����
	complex<double>* temp;
	// layer������е����㷨�Ĳ���
	for(layer = 0; layer < r; layer++)
	{
		// times����ǰ������Ҫ���еĵ��α任�Ĵ���������ؿ������Ϊ�������ĸ���
		for(times = 0; times < 1 << layer; times++)
		{
			// group_size����ÿ�ε��α任�Ĺ�ģ��С������ؿ������Ϊ�������Ĵ�С
			group_size = 1 << (r - layer);
			for(i = 0; i < group_size / 2; i++)
			{
				// index�����±���㣬���磺������еڶ��ε��α任���±���Ҫ�����Ѿ����еĵ��α任���� * ���ε��α任�Ĺ�ģ��С
				index = times * group_size;
				X2[index + i] = X1[index + i] + X1[index + group_size / 2 + i];
				X2[index + group_size / 2 + i] = (X1[index + i] - X1[index + group_size / 2 + i]) * W[i * (1 << layer)];
			}
		}
		// ÿһ��������֮������X1������һ��X2�õ���������
		temp = X1;
		X1 = X2;
		X2 = temp;
	}

	// ����������λ�������У�
	for(i = 0; i < count; i++)
	{
		k = 0;
		for(j = 0; j < r; j++)
		{
			if(i & (1 << j))
			{
				k += 1 << (r - j - 1);
			}
		}
		FD[i] = X1[k];
		FD[i] /= count;
	}

	//�ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}

// һά��ɢ���ٸ���Ҷ���任
void IFFT(complex<double>* FD, complex<double>* TD, int r)
{
	// ���㸵��Ҷ���任����
	long count;
	count = 1 << r;

	// ������������洢��
	complex<double> * X = new complex<double>[count];

	// ��Ƶ��ռ�����д��X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// ��Ƶ��ռ����ݵĹ����
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ��ռ����ݵĹ����
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// �ͷ��ڴ�
	delete X;

}

// ���ٸ���Ҷ�任
void Fast_Fourier()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* TD = new complex<double>[FFT_w * FFT_h]; //����ռ�
	complex<double>* FD = new complex<double>[FFT_w * FFT_h]; //Ƶ��ռ�

	int i,j;
    BYTE* pixel;
	int special_1 = 356;
	int special_2 = 206;

    // ������к���ȫ����Ϊ0
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			if(i >= h || j >= w)
				TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
			else if( j == 200)// && i == 50 && i<= 200)//(j >= special_2 && j <= special_1 && i == 200)
			{
				// ����ͼ��Ҷ�ֵ
				pixel = lpBits + LineBytes * (h - 1 - i) + j;
				// TD[w * i + j] = complex<double>(*pixel, 0);  //����Ҷ�ֵ
				// ƽ�Ʋ���
				//TD[i * FFT_w + j] = complex<double>(*pixel * pow(-1, i + j), 0);
				TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
			}
			else
			{
				TD[i * FFT_w + j] = complex<double>(255 * pow(-1, i + j), 0);
			}
		}
	}

	// ��ˮƽ���������ٸ���Ҷ�任
	for(i = 0; i < FFT_h; i++)
	{
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], width_index);
	}

	//�ش�ֱ����������Ҷ���任ʱ����Ϊ���ص�ĻҶ�ֵ����������һ��С��������ת�ã���������TD���ڴ�ռ����洢ת�õ�TD
	//ת��
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			TD[FFT_h * j + i] = FD[FFT_w * i + j];
		}
	}

	// �ش�ֱ���������ٸ���Ҷ�任
	for(i = 0; i < FFT_w; i++)
	{
		FFT(&TD[FFT_h * i], &FD[FFT_h * i], height_index);
	}

	// ɾ����ʱ����
	delete TD;

	// ����Ƶ��ͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // ��Ϣͷ+��ɫ�� // 1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
    lpDIB_FFT = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_FFT�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_FFT)
		return;

	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_FFT, lpBitsInfo, 1064);

	// �ı�ԭͼ���С
	lpDIB_FFT->bmiHeader.biWidth = FFT_w;
	lpDIB_FFT->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_FFT->bmiColors[lpDIB_FFT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}
	// ��tempFD����FD���ڷ��任
	tempFD = FD;
	
	// ������ڷ��任ͼ����free��
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	// �任��ɣ����и���Ҷ�任���
	Fast_Fourier_stage = 1;
}

// 24λ���ͼ����ٸ���Ҷ�任
void Color_Fast_Fourier()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* R_TD = new complex<double>[FFT_w * FFT_h]; //����ռ�
	complex<double>* G_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* R_FD = new complex<double>[FFT_w * FFT_h]; //Ƶ��ռ�
	complex<double>* G_FD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_FD = new complex<double>[FFT_w * FFT_h];

	int i,j;
    BYTE* pixel;

    // ������к���ȫ����Ϊ0
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			if(i >= h || j >= w)
			{
				R_TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
			    G_TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
				B_TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
			}
			else
			{
				// ����ͼ��RGBֵ
				pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
				// TD[w * i + j] = complex<double>(*pixel, 0);  //����Ҷ�ֵ
				// ƽ�Ʋ���
				B_TD[i * FFT_w + j] = complex<double>(*pixel * pow(-1, i + j), 0);
				G_TD[i * FFT_w + j] = complex<double>(*(pixel + 1) * pow(-1, i + j), 0);
				R_TD[i * FFT_w + j] = complex<double>(*(pixel + 2) * pow(-1, i + j), 0);
			}
		}
	}

	// ��ˮƽ���������ٸ���Ҷ�任
	for(i = 0; i < FFT_h; i++)
	{
		FFT(&B_TD[FFT_w * i], &B_FD[FFT_w * i], width_index);
		FFT(&G_TD[FFT_w * i], &G_FD[FFT_w * i], width_index);
		FFT(&R_TD[FFT_w * i], &R_FD[FFT_w * i], width_index);
	}

	//�ش�ֱ����������Ҷ���任ʱ����Ϊ���ص�ĻҶ�ֵ����������һ��С��������ת�ã���������TD���ڴ�ռ����洢ת�õ�TD
	//ת��
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			B_TD[FFT_h * j + i] = B_FD[FFT_w * i + j];
			G_TD[FFT_h * j + i] = G_FD[FFT_w * i + j];
			R_TD[FFT_h * j + i] = R_FD[FFT_w * i + j];
		}
	}

	// �ش�ֱ���������ٸ���Ҷ�任
	for(i = 0; i < FFT_w; i++)
	{
		FFT(&B_TD[FFT_h * i], &B_FD[FFT_h * i], height_index);
		FFT(&G_TD[FFT_h * i], &G_FD[FFT_h * i], height_index);
		FFT(&R_TD[FFT_h * i], &R_FD[FFT_h * i], height_index);
	}

	// ɾ����ʱ����
	delete B_TD;
	delete G_TD;
	delete R_TD;

	// ����Ƶ��ͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // ��Ϣͷ 24���û�е�ɫ��
    lpDIB_FFT = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_FFT�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_FFT)
		return;

	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_FFT, lpBitsInfo, 40);

	// �ı�ԭͼ���С
	lpDIB_FFT->bmiHeader.biWidth = FFT_w;
	lpDIB_FFT->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_FFT->bmiColors[lpDIB_FFT->bmiHeader.biClrUsed];

	double R_temp;
	double G_temp;
	double B_temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j * 3;
			B_temp = sqrt(B_FD[j * FFT_h + i].real() * B_FD[j * FFT_h + i].real() + B_FD[j * FFT_h + i].imag() * B_FD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
			G_temp = sqrt(G_FD[j * FFT_h + i].real() * G_FD[j * FFT_h + i].real() + G_FD[j * FFT_h + i].imag() * G_FD[j * FFT_h + i].imag()) * 2000;
			R_temp = sqrt(R_FD[j * FFT_h + i].real() * R_FD[j * FFT_h + i].real() + R_FD[j * FFT_h + i].imag() * R_FD[j * FFT_h + i].imag()) * 2000;
            if(B_temp > 255)
			    B_temp = 255;
			if(G_temp > 255)
			    G_temp = 255;
			if(R_temp > 255)
			    R_temp = 255;
            *pixel = (BYTE)B_temp;
			*(pixel + 1) = (BYTE)G_temp;
			*(pixel + 2) = (BYTE)R_temp;
		}
	}
	// ��tempFD����FD���ڷ��任
	R_tempFD = R_FD;
	G_tempFD = G_FD;
	B_tempFD = B_FD;
	
	// ������ڷ��任ͼ����free��
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	// �任��ɣ����и���Ҷ�任���
	Fast_Fourier_stage = 1;
}

// ���ٸ���Ҷ���任
void IFast_Fourier()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* TD = new complex<double>[FFT_w * FFT_h]; //����ռ�
	complex<double>* FD = new complex<double>[FFT_w * FFT_h]; //Ƶ��ռ�

	int i,j;
    BYTE* pixel;

   // ���ڸ���Ҷ�任������ת�ã��ʶ���Ҫת�ø�ֵFD[i * FFT_w + j] = tempFD[j * FFT_h + i];
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			FD[i * FFT_w + j] = tempFD[j * FFT_h + i];
		}
	}

	// ��ˮƽ���������ٸ���Ҷ���任
	for(i = 0; i < FFT_h; i++)
	{
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], width_index);
	}

	//�ش�ֱ����������Ҷ���任ʱ����Ϊ���ص�ĻҶ�ֵ����������һ��С��������ת�ã���������FD���ڴ�ռ����洢ת�õ�TD
	//ת��
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			FD[FFT_h * j + i] = TD[FFT_w * i + j];
		}
	}

	// �ش�ֱ���������ٸ���Ҷ���任
	for(i = 0; i < FFT_w; i++)
	{
		IFFT(&FD[FFT_h * i], &TD[FFT_h * i], height_index);
	}

	// ����ԭͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // ��Ϣͷ+��ɫ�� // 1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
    lpDIB_IFFT = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_FFT�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_IFFT)
		return;
    
	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_IFFT, lpBitsInfo, 1064);

	// �ı�ԭͼ���С
	lpDIB_IFFT->bmiHeader.biWidth = FFT_w;
	lpDIB_IFFT->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_IFFT->bmiColors[lpDIB_IFFT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			if(i >= h || j >= w)
			{	
				*pixel = 255;
				continue;
			}
			temp = (TD[j * FFT_h + i].real() / pow(-1, i + j));
            if(temp < 0)
				temp = 0;
			if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}

	// �任��ɣ����и���Ҷ�任���
	Fast_Fourier_stage = 0;
	// �ͷ��ڴ�
	delete TD;
	delete FD;
	delete tempFD;
}

// 24λ���ͼ����ٸ���Ҷ���任
void Color_IFast_Fourier()
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* R_TD = new complex<double>[FFT_w * FFT_h]; //����ռ�
	complex<double>* G_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* R_FD = new complex<double>[FFT_w * FFT_h]; //Ƶ��ռ�
	complex<double>* G_FD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_FD = new complex<double>[FFT_w * FFT_h];


	int i,j;
    BYTE* pixel;

   // ���ڸ���Ҷ�任������ת�ã��ʶ���Ҫת�ø�ֵFD[i * FFT_w + j] = tempFD[j * FFT_h + i];
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			B_FD[i * FFT_w + j] = B_tempFD[j * FFT_h + i];
			G_FD[i * FFT_w + j] = G_tempFD[j * FFT_h + i];
			R_FD[i * FFT_w + j] = R_tempFD[j * FFT_h + i];
		}
	}

	// ��ˮƽ���������ٸ���Ҷ���任
	for(i = 0; i < FFT_h; i++)
	{
		IFFT(&B_FD[FFT_w * i], &B_TD[FFT_w * i], width_index);
		IFFT(&G_FD[FFT_w * i], &G_TD[FFT_w * i], width_index);
		IFFT(&R_FD[FFT_w * i], &R_TD[FFT_w * i], width_index);
	}

	//�ش�ֱ����������Ҷ���任ʱ����Ϊ���ص�ĻҶ�ֵ����������һ��С��������ת�ã���������FD���ڴ�ռ����洢ת�õ�TD
	//ת��
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			B_FD[FFT_h * j + i] = B_TD[FFT_w * i + j];
			G_FD[FFT_h * j + i] = G_TD[FFT_w * i + j];
			R_FD[FFT_h * j + i] = R_TD[FFT_w * i + j];
		}
	}

	// �ش�ֱ���������ٸ���Ҷ���任
	for(i = 0; i < FFT_w; i++)
	{
		IFFT(&B_FD[FFT_h * i], &B_TD[FFT_h * i], height_index);
		IFFT(&G_FD[FFT_h * i], &G_TD[FFT_h * i], height_index);
		IFFT(&R_FD[FFT_h * i], &R_TD[FFT_h * i], height_index);
	}

	// ����ԭͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // ��Ϣͷ 24λ���ͼ��û����Ϣͷ
    lpDIB_IFFT = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_FFT�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_IFFT)
		return;
    
	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_IFFT, lpBitsInfo, 40);

	// �ı�ԭͼ���С
	lpDIB_IFFT->bmiHeader.biWidth = FFT_w;
	lpDIB_IFFT->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_IFFT->bmiColors[lpDIB_IFFT->bmiHeader.biClrUsed];

	double R_temp;
	double G_temp;
	double B_temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j * 3;
			if(i >= h || j >= w)
			{	
				*pixel = 255;
				*(pixel + 1) = 255;
				*(pixel + 2) = 255;
				continue;
			}
			B_temp = (B_TD[j * FFT_h + i].real() / pow(-1, i + j));
			G_temp = (G_TD[j * FFT_h + i].real() / pow(-1, i + j));
			R_temp = (R_TD[j * FFT_h + i].real() / pow(-1, i + j));
			// blue
            if(B_temp < 0)
				B_temp = 0;
			if(B_temp > 255)
			    B_temp = 255;
			// green
			if(G_temp < 0)
				G_temp = 0;
			if(G_temp > 255)
			    G_temp = 255;
			// red
			if(R_temp < 0)
				R_temp = 0;
			if(R_temp > 255)
			    R_temp = 255;
            *pixel = (BYTE)B_temp;
			*(pixel + 1) = (BYTE)G_temp;
			*(pixel + 2) = (BYTE)R_temp;
		}
	}

	// �任��ɣ����и���Ҷ�任���
	Fast_Fourier_stage = 0;
	// �ͷ��ڴ�
	delete R_TD;
	delete R_FD;
	delete R_tempFD;

	delete G_TD;
	delete G_FD;
	delete G_tempFD;

	delete B_TD;
	delete B_FD;
	delete B_tempFD;
}

// �����ǰ�ͻ���
void Clear()
{
    Fast_Fourier_stage = 0;
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
}

//-------------ģ�庯��---------------//
void Template(int* Array, float coef)
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// Ϊ��ͼ������ڴ�
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;

	// �����ڴ�ʧ���򷵻�
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// ����BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// �ҵ���ͼ��������ʼλ��
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l;
	BYTE *pixel, *new_pixel;
	float result;
	// ����ʹ����һ��3x3��ģ�壬�ʶ������ǵ�һ�к����һ���Լ���һ�к����һ��
	// ��(��ȥ��Ե����)
	for(i = 1; i < h - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = 1; j < w - 1; j++)
		{
			// ָ����ͼ��i�У���j�����ص�ָ��
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			result = 0;
			// ����3x3ģ�������ػҶ�ֵ�ĺ�
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					// �Ҷ�ֵ����ģ��ϵ�����ۼ�
					result += (*pixel) * Array[k * 3 + l];
				}
			}
			// �������ϵ��
			result *= coef;
			if (result < 0)
				*new_pixel = 0;
			else if(result > 255)
				*new_pixel = 255;
			else
				*new_pixel = (BYTE) (result + 0.5);
		}
	}
}

//-------------24λ���ģ�庯��---------------//
void Color_Template(int* Array, float coef)
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// Ϊ��ͼ������ڴ�
	LONG size = sizeof(BITMAPINFOHEADER) + h * LineBytes; //24λ���ͼ��û�е�ɫ��

	// �����ڴ�ʧ���򷵻�
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// ����BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// �ҵ���ͼ��������ʼλ��
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l;
	BYTE *pixel, *new_pixel;
	float R_result;
	float G_result;
	float B_result;
	// ����ʹ����һ��3x3��ģ�壬�ʶ������ǵ�һ�к����һ���Լ���һ�к����һ��
	// ��(��ȥ��Ե����)
	for(i = 1; i < h - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = 1; j < w - 1; j++)
		{
			// ָ����ͼ��i�У���j�����ص�ָ��
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j * 3;
			R_result = 0;
			G_result = 0;
			B_result = 0;
			// ����3x3ģ�������ػҶ�ֵ�ĺ�
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + (j - 1 + l) * 3;
					// �Ҷ�ֵ����ģ��ϵ�����ۼ�
					B_result += (*pixel) * Array[k * 3 + l];
					G_result += (*(pixel + 1)) * Array[k * 3 + l];
					R_result += (*(pixel + 2)) * Array[k * 3 + l];
				}
			}
			// �������ϵ��
			R_result *= coef;
			G_result *= coef;
			B_result *= coef;
			// blue
			if (B_result < 0)
				*new_pixel = 0;
			else if(B_result > 255)
				*new_pixel = 255;
			else
				*new_pixel = (BYTE) (B_result + 0.5); //+0.5��������
			// green
			if (G_result < 0)
				*(new_pixel + 1) = 0;
			else if(G_result > 255)
				*(new_pixel + 1) = 255;
			else
				*(new_pixel + 1) = (BYTE) (G_result + 0.5); //+0.5��������
			// red
			if (R_result < 0)
				*(new_pixel + 2) = 0;
			else if(R_result > 255)
				*(new_pixel + 2) = 255;
			else
				*(new_pixel + 2) = (BYTE) (R_result + 0.5); //+0.5��������
		}
	}
}


// ��ֵ�˲�
void AverageFilter()
{
	int Array[9]; //3x3ģ��
	//��׼��ֵ�˲�
	Array[0] = 1;	Array[1] = 1;	Array[2] = 1;
	Array[3] = 1;	Array[4] = 1;	Array[5] = 1;
	Array[6] = 1;	Array[7] = 1;	Array[8] = 1;
	Template(Array, (float)1/9);
}

// 24λ���ͼ���ֵ�˲�
void Color_AverageFilter()
{
	int Array[9]; //3x3ģ��
	//��׼��ֵ�˲�
	Array[0] = 1;	Array[1] = 1;	Array[2] = 1;
	Array[3] = 1;	Array[4] = 1;	Array[5] = 1;
	Array[6] = 1;	Array[7] = 1;	Array[8] = 1;
	Color_Template(Array, (float)1/9);
}

// ��˹�˲�
void GaussFilter()
{
	int Array[9]; //3x3ģ��
	//��˹�˲�ģ��
	Array[0] = 1;	Array[1] = 2;	Array[2] = 1;
	Array[3] = 2;	Array[4] = 4;	Array[5] = 2;
	Array[6] = 1;	Array[7] = 2;	Array[8] = 1;
	Template(Array, (float)1/16);
}

// 24λ��ʸ�˹�˲�
void Color_GaussFilter()
{
	int Array[9]; //3x3ģ��
	//��˹�˲�ģ��
	Array[0] = 1;	Array[1] = 2;	Array[2] = 1;
	Array[3] = 2;	Array[4] = 4;	Array[5] = 2;
	Array[6] = 1;	Array[7] = 2;	Array[8] = 1;
	Color_Template(Array, (float)1/16);
}


//-------------��ֵ�˲�����---------------//
BYTE WINAPI GetMediumNum(BYTE* Array, int Array_size)
{
	int i,j;
	BYTE temp;
	// ��ð�ݷ��������������
	for (j = 0; j < Array_size - 1; j ++)
	{
		for (i = 0; i < Array_size - j - 1; i ++)
		{
			if (Array[i] > Array[i + 1])
			{
				temp = Array[i];
				Array[i] = Array[i + 1];
				Array[i + 1] = temp;
			}
		}
	}
	// ������ֵ
	return Array[(Array_size - 1) / 2];
}


// ��ֵ�˲�
void MediumFilter()
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// Ϊ��ͼ������ڴ�
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;

	// ����ڴ����ʧ���򷵻�
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// ����BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	// �ҵ���ͼ��������ʼλ��
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, Array_width, Array_height, Array_size;
	BYTE *pixel, *new_pixel;
	BYTE Value[9]; //3x3ģ��
	Array_width = 3; //ģ��Ŀ�
	Array_height = 3; //ģ��ĸ�
	Array_size = Array_width * Array_height; //��¼ģ���С
	// ��(��ȥ��Ե����)
	for(i = 1; i < h - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = 1; j < w - 1; j++)
		{
			// ָ����ͼ��i�У���j�����ص�ָ��
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			// ���� 3x3ģ�������صĻҶ�ֵ
			for (k = 0; k < Array_height; k++)
			{
				for (l = 0; l < Array_width; l++)
				{
					// ָ��ԭͼ��i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					// ��������ֵ
					Value[k * Array_height + l] = *pixel;
				}
			}
			// ��ȡ��ֵ
			*new_pixel = GetMediumNum(Value, Array_size);
		}
	}
}


// 24λ���ͼ����ֵ�˲�
void Color_MediumFilter()
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// Ϊ��ͼ������ڴ�
	LONG size = sizeof(BITMAPINFOHEADER) + h * LineBytes; //24λͼ��û�е�ɫ��

	// ����ڴ����ʧ���򷵻�
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// ����BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	// �ҵ���ͼ��������ʼλ��
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, Array_width, Array_height, Array_size;
	BYTE *pixel, *new_pixel;
	BYTE R_Value[9]; //3x3ģ��
	BYTE G_Value[9]; //3x3ģ��
	BYTE B_Value[9]; //3x3ģ��
	Array_width = 3; //ģ��Ŀ�
	Array_height = 3; //ģ��ĸ�
	Array_size = Array_width * Array_height; //��¼ģ���С
	// ��(��ȥ��Ե����)
	for(i = 1; i < h - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = 1; j < w - 1; j++)
		{
			// ָ����ͼ��i�У���j�����ص�ָ��
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j * 3;
			// ���� 3x3ģ�������صĻҶ�ֵ
			for (k = 0; k < Array_height; k++)
			{
				for (l = 0; l < Array_width; l++)
				{
					// ָ��ԭͼ��i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + (j - 1 + l) * 3;
					// ��������ֵ
					B_Value[k * Array_height + l] = *pixel;
					G_Value[k * Array_height + l] = *(pixel + 1);
					R_Value[k * Array_height + 2] = *(pixel + 2);
				}
			}
			// ��ȡ��ֵ
			*new_pixel = GetMediumNum(B_Value, Array_size);
			*(new_pixel + 1) = GetMediumNum(G_Value, Array_size);
			*(new_pixel + 2) = GetMediumNum(R_Value, Array_size);
		}
	}
}


// ������˹��
void LaplaceSharpening()
{
	int Array[9]; //3x3ģ��
/*
	//������˹����
	Array[0] = 0;	Array[1] = 1;	Array[2] = 0;
	Array[3] = 1;	Array[4] = -4;	Array[5] = 1;
	Array[6] = 0;	Array[7] = 1;	Array[8] = 0;
*/	
	//������˹��
	Array[0] = -1;	Array[1] = -1;	Array[2] = -1;
	Array[3] = -1;	Array[4] = 9;	Array[5] = -1;
	Array[6] = -1;	Array[7] = -1;	Array[8] = -1;
	Template(Array, (float)1);
}

// 24λ���ͼ��������˹��
void Color_LaplaceSharpening()
{
	int Array[9]; //3x3ģ��
/*
	//������˹����
	Array[0] = 0;	Array[1] = 1;	Array[2] = 0;
	Array[3] = 1;	Array[4] = -4;	Array[5] = 1;
	Array[6] = 0;	Array[7] = 1;	Array[8] = 0;
*/	
	//������˹��
	Array[0] = -1;	Array[1] = -1;	Array[2] = -1;
	Array[3] = -1;	Array[4] = 9;	Array[5] = -1;
	Array[6] = -1;	Array[7] = -1;	Array[8] = -1;
	Color_Template(Array, (float)1);
}

// 8λ�Ҷ�ͼ��Robert��ģ��
void TemplateForRobert(int* Array_1, int* Array_2, float coef)
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// Ϊ��ͼ������ڴ�
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;

	// �����ڴ�ʧ���򷵻�
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// ����BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// �ҵ���ͼ��������ʼλ��
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, m, n;
	BYTE *pixel, *new_pixel;
	float result, result_1, result_2;
	// ����ʹ����һ��3x3��ģ�壬�ʶ������ǵ�һ�к����һ���Լ���һ�к����һ��
	// ��(��ȥ��Ե����)
	for(i = 1; i < h - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = 1; j < w - 1; j++)
		{
			// ָ����ͼ��i�У���j�����ص�ָ��
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			result = 0;
			result_1 = 0;
			result_2 = 0;
			// ����3x3ģ�������ػҶ�ֵ�ĺ�
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					// �Ҷ�ֵ����ģ��ϵ�����ۼ�
					result_1 += (*pixel) * Array_1[k * 3 + l];
				}
			}

			for (m = 0; m < 3; m++)
			{
				for (n = 0; n < 3; n++)
				{
					// ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - m) + j - 1 + n;
					// �Ҷ�ֵ����ģ��ϵ�����ۼ�
					result_2 += (*pixel) * Array_2[m * 3 + n];
				}
			}
			if(result_1 < 0)
			    result_1 = result_1 * (-1);
			if(result_2 < 0)
				result_2 = result_2 * (-1);

            result = result_1 + result_2;
			// �������ϵ��
			result *= coef;
			if (result < 0)
				*new_pixel = 0;
			else if(result > 255)
				*new_pixel = 255;
			else
				*new_pixel = (BYTE) (result + 0.5);
		}
	}
}

// 24λ���ͼ��Robert��ģ��
void Color_TemplateForRobert(int* Array_1, int* Array_2, float coef)
{
    if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// Ϊ��ͼ������ڴ�
	LONG size = sizeof(BITMAPINFOHEADER) + h * LineBytes; // 24λ���ͼ��û�е�ɫ��

	// �����ڴ�ʧ���򷵻�
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// ����BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// �ҵ���ͼ��������ʼλ��
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, m, n;
	BYTE *pixel, *new_pixel;
	float R_result, R_result_1, R_result_2;
	float G_result, G_result_1, G_result_2;
	float B_result, B_result_1, B_result_2;
	// ����ʹ����һ��3x3��ģ�壬�ʶ������ǵ�һ�к����һ���Լ���һ�к����һ��
	// ��(��ȥ��Ե����)
	for(i = 1; i < h - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = 1; j < w - 1; j++)
		{
			// ָ����ͼ��i�У���j�����ص�ָ��
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j * 3;
			// red
			R_result = 0;
			R_result_1 = 0;
			R_result_2 = 0;
			// green
			G_result = 0;
			G_result_1 = 0;
			G_result_2 = 0;
			// blue
			B_result = 0;
			B_result_1 = 0;
			B_result_2 = 0;
			// ����3x3ģ�������ػҶ�ֵ�ĺ�
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + (j - 1 + l) * 3;
					// �Ҷ�ֵ����ģ��ϵ�����ۼ�
					B_result_1 += (*pixel) * Array_1[k * 3 + l];
					G_result_1 += (*(pixel + 1)) * Array_1[k * 3 + l];
					R_result_1 += (*(pixel + 2)) * Array_1[k * 3 + l];
				}
			}

			for (m = 0; m < 3; m++)
			{
				for (n = 0; n < 3; n++)
				{
					// ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i - 1 + k�У���j - 1 + l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - m) + (j - 1 + n) * 3;
					// �Ҷ�ֵ����ģ��ϵ�����ۼ�
					B_result_2 += (*pixel) * Array_2[m * 3 + n];
					G_result_2 += (*(pixel + 1)) * Array_2[m * 3 + n];
					R_result_2 += (*(pixel + 2)) * Array_2[m * 3 + n];
				}
			}
			// blue
			if(B_result_1 < 0)
			    B_result_1 = B_result_1 * (-1);
			if(B_result_2 < 0)
				B_result_2 = B_result_2 * (-1);

			// green
			if(G_result_1 < 0)
			    G_result_1 = G_result_1 * (-1);
			if(G_result_2 < 0)
				G_result_2 = G_result_2 * (-1);

			// red
			if(R_result_1 < 0)
			    R_result_1 = R_result_1 * (-1);
			if(R_result_2 < 0)
				R_result_2 = R_result_2 * (-1);

            B_result = B_result_1 + B_result_2;
			G_result = G_result_1 + G_result_2;
			R_result = R_result_1 + R_result_2;
			// �������ϵ��
			B_result *= coef;
			G_result *= coef;
			R_result *= coef;

			// blue
			if (B_result < 0)
				*new_pixel = 0;
			else if(B_result > 255)
				*new_pixel = 255;
			else
				*new_pixel = (BYTE) (B_result + 0.5);

			// green
			if (G_result < 0)
				*(new_pixel + 1) = 0;
			else if(G_result > 255)
				*(new_pixel + 1) = 255;
			else
				*(new_pixel + 1) = (BYTE) (G_result + 0.5);

			// red
			if (R_result < 0)
				*(new_pixel + 2) = 0;
			else if(R_result > 255)
				*(new_pixel + 2) = 255;
			else
				*(new_pixel + 2) = (BYTE) (R_result + 0.5);
		}
	}
}

// �ݶ��񻯡���Robert��
void GradSharpening()
{
	int Array_1[9]; //3x3ģ��
	int Array_2[9]; //3x3ģ��
	//ʹ���ݶȽ��Ƽ������һ����ʽ����Robert����
	Array_1[0] = 0;	Array_1[1] = 0;	Array_1[2] = 0;
	Array_1[3] = 0;	Array_1[4] = 1;	Array_1[5] = 0;
	Array_1[6] = 0;	Array_1[7] = 0; Array_1[8] = -1;

	Array_2[0] = 0;	Array_2[1] = 0;	Array_2[2] = 0;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 1;
	Array_2[6] = 0;	Array_2[7] = -1; Array_2[8] = 0;

	TemplateForRobert(Array_1, Array_2, (float)1.1);
}

// 24λ���ͼ��
// �ݶ��񻯡���Robert��
void Color_GradSharpening()
{
	int Array_1[9]; //3x3ģ��
	int Array_2[9]; //3x3ģ��
	//ʹ���ݶȽ��Ƽ������һ����ʽ����Robert����
	Array_1[0] = 0;	Array_1[1] = 0;	Array_1[2] = 0;
	Array_1[3] = 0;	Array_1[4] = 1;	Array_1[5] = 0;
	Array_1[6] = 0;	Array_1[7] = 0; Array_1[8] = -1;

	Array_2[0] = 0;	Array_2[1] = 0;	Array_2[2] = 0;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 1;
	Array_2[6] = 0;	Array_2[7] = -1; Array_2[8] = 0;

	Color_TemplateForRobert(Array_1, Array_2, (float)1.1);
}

// �ݶ��񻯡���Priwitt��
void PriwittSharpening()
{
	int Array_1[9]; //3x3ģ��
	int Array_2[9]; //3x3ģ��
	//ʹ���ݶȽ��Ƽ������һ����ʽ����Robert����
	Array_1[0] = -1;	Array_1[1] = 0;	Array_1[2] = 1;
	Array_1[3] = -1;	Array_1[4] = 0;	Array_1[5] = 1;
	Array_1[6] = -1;	Array_1[7] = 0; Array_1[8] = 1;

	Array_2[0] = -1; Array_2[1] = -1; Array_2[2] = -1;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 0;
	Array_2[6] = 1;	Array_2[7] = 1; Array_2[8] = 1;

	TemplateForRobert(Array_1, Array_2, (float)1);
}

// 24λ���ͼ��
// �ݶ��񻯡���Priwitt��
void Color_PriwittSharpening()
{
	int Array_1[9]; //3x3ģ��
	int Array_2[9]; //3x3ģ��
	//ʹ���ݶȽ��Ƽ������һ����ʽ����Robert����
	Array_1[0] = -1;	Array_1[1] = 0;	Array_1[2] = 1;
	Array_1[3] = -1;	Array_1[4] = 0;	Array_1[5] = 1;
	Array_1[6] = -1;	Array_1[7] = 0; Array_1[8] = 1;

	Array_2[0] = -1; Array_2[1] = -1; Array_2[2] = -1;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 0;
	Array_2[6] = 1;	Array_2[7] = 1; Array_2[8] = 1;

	Color_TemplateForRobert(Array_1, Array_2, (float)1);
}

//----------------����Ƶ���˲�����---------------//
//choose=0 ��ͨ 1 ��ͨ 
void IdealFilter(int choose)
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	// ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	    
	// ��ȡͼ������
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // �˲��뾶��ƽ��
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	} 
    else
	{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// �˲�
	int i, j;
	double d2;
	r2 = pow(150 ,2);
	if(choose)
	{
	    r2 *= 0.05;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				if(d2 < r2)
	        		tempFD[i + FFT_h * j] *= 0; //������Ƶ����ʹͼ��ɿ�
			}
		}
	}
	else
	{
		r2 *= 0.05;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				if(d2 > r2)
					tempFD[i + FFT_h * j] = 0;
			}
		}
	}

	BYTE* pixel;
	// ����Ƶ��ͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // ��Ϣͷ+��ɫ�� // 1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
    BITMAPINFO* lpDIB_Ideal = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_Ideal�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_Ideal)
		return;

	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_Ideal, lpBitsInfo, 1064);

	// �ı�ԭͼ���С
	lpDIB_Ideal->bmiHeader.biWidth = FFT_w;
	lpDIB_Ideal->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_Ideal->bmiColors[lpDIB_Ideal->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			temp = sqrt(tempFD[j * FFT_h + i].real() * tempFD[j * FFT_h + i].real() + tempFD[j * FFT_h + i].imag() * tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}
	// ��ԭ����Ƶ��ͼ������ʾ
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Ideal;
	}
}

// 24λ���ͼ��
//----------------����Ƶ���˲�����---------------//
//choose=0 ��ͨ 1 ��ͨ 
void Color_IdealFilter(int choose)
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	// ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	    
	// ��ȡͼ������
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // �˲��뾶��ƽ��
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	} 
    else
	{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// �˲�
	int i, j;
	double d2;
	r2 = pow(150 ,2);
	if(choose)
	{
	    r2 *= 0.05;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				if(d2 < r2)
				{
	        		R_tempFD[i + FFT_h * j] *= 0; //������Ƶ����ʹͼ��ɿ�
				    G_tempFD[i + FFT_h * j] *= 0;
					B_tempFD[i + FFT_h * j] *= 0;
				}
			}
		}
	}
	else
	{
		r2 *= 0.05;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				if(d2 > r2)
				{
					R_tempFD[i + FFT_h * j] = 0;
					G_tempFD[i + FFT_h * j] = 0;
					B_tempFD[i + FFT_h * j] = 0;
				}
			}
		}
	}

	BYTE* pixel;
	// ����Ƶ��ͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // ��Ϣͷ 24λ���ͼ��û�е�ɫ��
    BITMAPINFO* lpDIB_Ideal = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_Ideal�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_Ideal)
		return;

	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_Ideal, lpBitsInfo, 40);

	// �ı�ԭͼ���С
	lpDIB_Ideal->bmiHeader.biWidth = FFT_w;
	lpDIB_Ideal->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_Ideal->bmiColors[lpDIB_Ideal->bmiHeader.biClrUsed];

	double R_temp;
	double G_temp;
	double B_temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j * 3;
			// blue
	    	B_temp = sqrt(B_tempFD[j * FFT_h + i].real() * B_tempFD[j * FFT_h + i].real() + B_tempFD[j * FFT_h + i].imag() * B_tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(B_temp > 255)
			    B_temp = 255;
            *pixel = (BYTE)B_temp;

			// green
	    	G_temp = sqrt(G_tempFD[j * FFT_h + i].real() * G_tempFD[j * FFT_h + i].real() + G_tempFD[j * FFT_h + i].imag() * G_tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(G_temp > 255)
			    G_temp = 255;
            *(pixel + 1) = (BYTE)G_temp;

			// red
	    	R_temp = sqrt(R_tempFD[j * FFT_h + i].real() * R_tempFD[j * FFT_h + i].real() + R_tempFD[j * FFT_h + i].imag() * R_tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(R_temp > 255)
			    R_temp = 255;
            *(pixel + 2) = (BYTE)R_temp;
		}
	}
	// ��ԭ����Ƶ��ͼ������ʾ
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Ideal;
	}
}

//-------------������˹Ƶ���˲�����--------------//
//choose=0 ��ͨ 1 ��ͨ, nΪ����
void ButterworthFilter(int choose)
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}
    
	// ����
	int n=1;

	// ��ȡͼ������
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // �˲��뾶��ƽ��
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	}
	else{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// �˲�
	int i, j;
	double d2;
	if(choose)
	{
		r2 *= 0.01;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(r2 / d2, n)) + 0.8;
			}
		}
		
	}
	else
	{
		r2 *= 0.01;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(d2 / r2, n));
			}
		}
	}

    BYTE* pixel;
	// ����Ƶ��ͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // ��Ϣͷ+��ɫ�� // 1024����256λɫ��*4�õ�(RGBռ�����ֽڣ�Reservedռ��һ���ֽ�)
    BITMAPINFO* lpDIB_Butterworth = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_Butterworth�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_Butterworth)
		return;

	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_Butterworth, lpBitsInfo, 1064);

	// �ı�ԭͼ���С
	lpDIB_Butterworth->bmiHeader.biWidth = FFT_w;
	lpDIB_Butterworth->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_Butterworth->bmiColors[lpDIB_Butterworth->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			temp = sqrt(tempFD[j * FFT_h + i].real() * tempFD[j * FFT_h + i].real() + tempFD[j * FFT_h + i].imag() * tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}
	// ��ԭ����Ƶ��ͼ������ʾ
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Butterworth;
	}
}

// 24λ���ͼ��
//-------------������˹Ƶ���˲�����--------------//
//choose=0 ��ͨ 1 ��ͨ, nΪ����
void Color_ButterworthFilter(int choose)
{
	if(NULL == lpBitsInfo)// ͼ��δ���أ�����
		return;

	//ͼ���ȡ�Ĳ���
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ���ָ���������е��α任�Ĳ���
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// �����ȡ��ͼ�����2�������η�������Ҫ�ٳ���2��ΪFFT_w������ԭ����ȵĲ�����0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ���ָ���������е��α任�Ĳ���
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// �����ȡ��ͼ��߲���2�������η�������Ҫ�ٳ���2��ΪFFT_h������ԭ���߶ȵĲ�����0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}
    
	// ����
	int n=1;

	// ��ȡͼ������
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // �˲��뾶��ƽ��
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	}
	else{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// �˲�
	int i, j;
	double d2;
	if(choose)
	{
		r2 *= 0.01;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				R_tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(r2 / d2, n)) + 0.8;
				G_tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(r2 / d2, n)) + 0.8;
				B_tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(r2 / d2, n)) + 0.8;
			}
		}
		
	}
	else
	{
		r2 *= 0.01;
		for (i = 0; i < FFT_h; i ++)
		{
			for (j = 0; j < FFT_w; j ++)
			{
				d2 = pow(i - centerWidth, 2) + pow(j - centerHeight, 2);
				R_tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(d2 / r2, n));
				G_tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(d2 / r2, n));
				B_tempFD[i + FFT_h * j] *= 1.0 / (1 + pow(d2 / r2, n));
			}
		}
	}

    BYTE* pixel;
	// ����Ƶ��ͼ��
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // ��Ϣͷ 24λ���ͼ��û�е�ɫ��
    BITMAPINFO* lpDIB_Butterworth = (LPBITMAPINFO)malloc(size);

	// ��ӱ��������lpDIB_Butterworth�����ڴ�ʧ�ܣ��򷵻�
	if(NULL == lpDIB_Butterworth)
		return;
	
	// ����ԭͼ���ļ�ͷ����Ϣͷ
	memcpy(lpDIB_Butterworth, lpBitsInfo, 40);

	// �ı�ԭͼ���С
	lpDIB_Butterworth->bmiHeader.biWidth = FFT_w;
	lpDIB_Butterworth->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_Butterworth->bmiColors[lpDIB_Butterworth->bmiHeader.biClrUsed];

	double R_temp;
	double G_temp;
	double B_temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j * 3;
			R_temp = sqrt(R_tempFD[j * FFT_h + i].real() * R_tempFD[j * FFT_h + i].real() + R_tempFD[j * FFT_h + i].imag() * R_tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(R_temp > 255)
			    R_temp = 255;
            *(pixel + 2) = (BYTE)R_temp;

			G_temp = sqrt(G_tempFD[j * FFT_h + i].real() * G_tempFD[j * FFT_h + i].real() + G_tempFD[j * FFT_h + i].imag() * G_tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(G_temp > 255)
			    G_temp = 255;
            *(pixel + 1) = (BYTE)G_temp;

			B_temp = sqrt(B_tempFD[j * FFT_h + i].real() * B_tempFD[j * FFT_h + i].real() + B_tempFD[j * FFT_h + i].imag() * B_tempFD[j * FFT_h + i].imag()) * 2000; // ��ǿ��������ǧ
            if(B_temp > 255)
			    B_temp = 255;
            *pixel = (BYTE)B_temp;
		}
	}
	// ��ԭ����Ƶ��ͼ������ʾ
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Butterworth;
	}
}
