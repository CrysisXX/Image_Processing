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

	fread(&bf, 14, 1, fp);//文件头
	fread(&bi, 40, 1, fp);//信息头指针，实际位图可以用这个计算出来

	//计算要为指针lpBitsInfo分配多少size
	DWORD NumColors;//调色板实际长度
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
			NumColors = 0;//真彩图像没有调色板
			break;
		}
	}

	DWORD LineBytes = (bi.biWidth * bi.biBitCount + 31) / 32 * 4;//每一行的字节数
	DWORD size = 40 + NumColors * 4 + LineBytes * bi.biHeight;//总大小 = 信息头长度 + 调色板长度 * 4 + 图像大小

	if(NULL == (lpBitsInfo = (BITMAPINFO*)malloc(size)))//为lpBitsInfo分配内存空间，大小为size
		return false;//如果分配失败，则返回false

	fseek(fp, 14, SEEK_SET);//重置指针lp1的位置
    fread((char*)lpBitsInfo, size, 1, fp);//读入数据
	fclose(fp);


	//LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//指针2的位置

	lpBitsInfo->bmiHeader.biClrUsed = NumColors;//重置图片所使用的颜色数量，即调色板的实际长度

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

//24位真彩图像转化为灰度图像
void gray_true()
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int new_LineBytes = (w * 8 + 31) / 32 * 4; //八位灰度图像的每一行的字节数
	LONG size = 40 + 1024 + new_LineBytes * h; //1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
	
	LPBITMAPINFO new_lpBitsInfo = (LPBITMAPINFO)malloc(size);//为指针1分配内存，大小为size

	memcpy(new_lpBitsInfo, lpBitsInfo, 40); //复制信息头
    new_lpBitsInfo->bmiHeader.biBitCount = 8; //修改图像位数
	new_lpBitsInfo->bmiHeader.biClrUsed = 256; //修改图像所使用的颜色数

	int i,j;
	for(i = 0; i < 256; i++) //填充调色板
	{
		new_lpBitsInfo->bmiColors[i].rgbRed = i;
        new_lpBitsInfo->bmiColors[i].rgbGreen = i;
		new_lpBitsInfo->bmiColors[i].rgbBlue = i;
		new_lpBitsInfo->bmiColors[i].rgbReserved = i;
	}

	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];//重定位新指针2的位置
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

//像素处理
bool IsGray()
{
	int judge_gray = 0;
    int first1,first2,first3;
    int mid1,mid2,mid3;
    int last1,last2,last3;
    first1 = first2 = first3 = 0;
    mid1 = mid2 = mid3 =0;
    last1 = last2 = last3 = 0;

	//判断灰度图像
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

    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	if(i >= h || j >= w)
		return; //如果鼠标点击范围超过图像范围，则直接返回


	if(i > h || j > w)// 鼠标点击范围超出图像显示返回，返回
		return;

	BYTE *pixel, bv;
	int r, g, b;
	int index = 0;//case 4不可以直接更改*pixel的值，需要定义一个新的值进行接收

	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
    case 24:
		pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;//(h-i-1)做取反处理
		b = *pixel;
		g = *(pixel + 1);
		r = *(pixel + 2);
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		break;
	
	case 8:
		pixel = lpBits + LineBytes * (h - 1 - i) + j;

		if(!IsGray())
		{
			r = lpBitsInfo->bmiColors[*pixel].rgbRed; //根据pixel索引查找8位图像的调色板，得到具体的RGB数值
			g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
			sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		}
		else
			sprintf(str, "灰度值：%d", *pixel);

		break;

	case 4:
		pixel = lpBits + LineBytes * (h - 1 - i) + j / 2 ;
		if(0 == j % 2)
			index = *pixel >> 4;//横坐标是偶数，查找的是该字节的高四位，向右移动4位
		else
			index = *pixel & 15;//横坐标是奇数，查找的是该字节的低四位，和00001111=15做与运算

	    r = lpBitsInfo->bmiColors[index].rgbRed; //根据pixel索引查找4位图像的调色板，得到具体的RGB数值
		g = lpBitsInfo->bmiColors[index].rgbGreen;
		b = lpBitsInfo->bmiColors[index].rgbBlue;
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);

		break;

	case 1:
		//j/8在哪个字节找
		//bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7  - j % 8));//内存形式，低四位，高四位 1,2,3,4|5,6,7,8//刚好能被8整除是第一位，余7反而是最后一位 0 1 2 3 4 5 6 7 | 8 9 10...
		bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7  - j % 8));

		if(0 == bv)
		{
			//r = lpBitsInfo->bmiColors[0].rgbRed; //根据pixel索引查找1位图像的调色板，得到具体的RGB数值
			//g = lpBitsInfo->bmiColors[0].rgbGreen;
			//b = lpBitsInfo->bmiColors[0].rgbBlue;
			//sprintf(str, "背景色RGB(%d,%d,%d)", r, g, b);
			sprintf(str, "背景色");
		}
		else
		{
			//r = lpBitsInfo->bmiColors[1].rgbRed; //根据pixel索引查找1位图像的调色板，得到具体的RGB数值
			//g = lpBitsInfo->bmiColors[1].rgbGreen;
			//b = lpBitsInfo->bmiColors[1].rgbBlue;
			sprintf(str, "前景色");
		}
		break;
	}
	//AfxMessageBox(str);
}

//添加全局灰度直方图记录数组
DWORD H[256];
void Histogram()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i,j;
	BYTE *pixel;

	//灰度直方图数组记录值初始化
	for(i = 0; i <256; i++)
	{
		H[i] = 0;
	}

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			//pixel指向8位灰度图像的像素点，索引值即为灰度值
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			H[*pixel]++;
        }
	}
}

// 24位真彩图像直方图
DWORD R[256];
DWORD G[256];
DWORD B[256];
void Color_Histogram()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i, j, r, g, b;
	BYTE *pixel;

	//三色直方图数组记录值初始化
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
			//pixel指向24位真彩图像图像的像素点
			pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
			//蓝色在前，绿色居中，红色最后
			b = *pixel;
			g = *(pixel + 1);
		    r = *(pixel + 2);
			B[b]++;
			G[g]++;
			R[r]++;
        }
	}
}

//8位灰度图像点运算
void LineTrans()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	float a,b;
	float bv;
	BYTE *pixel;
	//设置曲线参数
	//斜率
	a = 1.1f;
	//截距
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
			*pixel = (BYTE)(bv + 0.5); //四舍五入
		}
	}

}

//24位真彩图像点运算
//Slope斜率 Intercept截距
void Color_LineTrans(double Slope, double Intercept)
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	double R_pixel, G_pixel, B_pixel;
	BYTE *pixel;
	//设置曲线参数

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
            //+0.5四舍五入
			*pixel = (BYTE)(B_pixel + 0.5); 
			*(pixel + 1) = (BYTE)(G_pixel + 0.5);
			*(pixel + 2) = (BYTE)(R_pixel + 0.5);
		}
	}
}


// 8位灰度图像均衡化
void Equalize()
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	float temp;
	BYTE *pixel;
	//灰度映射表
	int Map[256];
	Histogram();
    
	//计算灰度映射表
    for(i = 0; i < 256; i++)
	{
		//每次统计都要给temp赋予初始值
		temp = 0;
		for(j = 0; j <= i; j++)
		{
            temp += H[j];
		}
		//四舍五入
		Map[i] = (BYTE)((255 * temp) / (w * h) + 0.5);
	}

	//均衡化
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = Map[*pixel];
		}
	}
}

// 24位真彩图像均衡化
void Color_Equalize()
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	float R_temp, G_temp, B_temp;
	BYTE *pixel;
	//灰度映射表
	int R_Map[256], G_Map[256], B_Map[256];
	Color_Histogram();
    
	//计算灰度映射表
    for(i = 0; i < 256; i++)
	{
		//每次统计都要给temp赋予初始值
		R_temp = 0;
		for(j = 0; j <= i; j++)
		{
            R_temp += R[j];
		}
		//四舍五入
		R_Map[i] = (BYTE)((255 * R_temp) / (w * h) + 0.5);
	}

	for(i = 0; i < 256; i++)
	{
		//每次统计都要给temp赋予初始值
		G_temp = 0;
		for(j = 0; j <= i; j++)
		{
            G_temp += G[j];
		}
		//四舍五入
		G_Map[i] = (BYTE)((255 * G_temp) / (w * h) + 0.5);
	}

	for(i = 0; i < 256; i++)
	{
		//每次统计都要给temp赋予初始值
		B_temp = 0;
		for(j = 0; j <= i; j++)
		{
            B_temp += B[j];
		}
		//四舍五入
		B_Map[i] = (BYTE)((255 * B_temp) / (w * h) + 0.5);
	}

	//均衡化
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
	// M代表需要进行傅立叶变换的点数
	for(u = 0; u < M; u++)
	{
		// 初始化FD为0
		FD[u] = 0;
		for(x = 0; x < M; x++)
		{
			angle = -2 * PI * u * x / M;
			// complex<double>(cos,sin)，前面是实部，后面是虚部
			// 欧拉公式 e^(i*angle) = cos(angle) + i * sin(angle)
			FD[u] += TD[x] * complex<double>(cos(angle), sin(angle)); 
		}
		FD[u] /= M;
	}
}

void IFT(complex<double>* TD, complex<double>* FD, int M)
{
	int u, x;
    double angle;
	// M代表需要进行傅立叶反变换的点数
	for(x = 0; x < M; x++)
	{
		// 初始化TD为0
		TD[x] = 0;
		for(u = 0; u < M; u++)
		{
			angle = 2 * PI * u * x / M;
			// complex<double>(cos,sin)，前面是实部，后面是虚部
			// 欧拉公式 e^(i*angle) = cos(angle) + i * sin(angle)
			TD[x] += FD[u] * complex<double>(cos(angle), sin(angle)); 
		}
	}
}

// 全局获取两次傅里叶变换之后的FD
complex<double>* tempFD;
// 全局获取两次傅里叶变换之后的FD
complex<double>* R_tempFD;
complex<double>* G_tempFD;
complex<double>* B_tempFD;

int Fourier_stage = 0;
int Fast_Fourier_stage = 0;

// 判断tempFD存在与否
bool FD_exist()
{
	if(tempFD && Fourier_stage)
		return true;
	else
		return false;
}

// 判断tempFD存在与否
bool Color_FD_exist()
{
	if(R_tempFD && G_tempFD && B_tempFD && Fast_Fourier_stage)
		return true;
	else
		return false;
}

void Fourier()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h]; //空域空间
	complex<double>* FD = new complex<double>[w * h]; //频域空间

	int i,j;
	BYTE* pixel;
	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			// 访问图像灰度值
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			// TD[w * i + j] = complex<double>(*pixel, 0);  //赋予灰度值
			// 平移操作
			TD[w * i + j] = complex<double>(*pixel * pow(-1, i + j), 0);
		}
	}

	for(i = 0; i < h; i++)
	{
		FT(&TD[w * i], &FD[w * i], w);  //沿水平方向，每一行都做傅里叶变换，一行有w个点
	}

	//沿垂直方向做傅里叶变换时，因为像素点的灰度值不连续，有一个小技巧是做转置，并且利用TD的内存空间来存储转置的TD
	//转置
    for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			TD[h * j + i] = FD[w * i + j];
		}
	}

	for(i = 0; i < w; i++)
	{
		FT(&TD[h * i], &FD[h * i], h);  //沿垂直方向，每一列都做傅里叶变换，一列有h个点
	}

	// 获取两次傅里叶变换的频域空间
	tempFD = FD;

	long size = 40 + 1024 + LineBytes * h; // 信息头+调色板 // 1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
    BITMAPINFO* lpDIB_FT = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_FT分配内存失败，则返回
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
			temp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + FD[j * h + i].imag() * FD[j * h + i].imag()) * 2000; // 增强，乘以两千
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}

	// 变换完成，进行傅里叶变换标记
	Fourier_stage = 1;
	// 释放内存
	delete TD;
	// 在原图像区域显示
	free(lpBitsInfo);
    lpBitsInfo = lpDIB_FT;
}

void IFourier()
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h]; //空域空间
	complex<double>* FD = new complex<double>[w * h]; //频域空间

	int i,j;
	BYTE* pixel;

	for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			FD[w * i + j] = tempFD[h * j + i];
		}
	}

	//沿水平方向，每一行都做傅里叶反变换，一行有w个点
	for(i = 0; i < h; i++)
	{
		IFT(&TD[w * i], &FD[w * i], w);  
	}

	//沿垂直方向做傅里叶反变换时，因为像素点的灰度值不连续，有一个小技巧是做转置，并且利用TD的内存空间来存储转置的TD
	//转置
    for(i = 0; i < h; i++)
	{
        for(j = 0; j < w; j++)
		{
			FD[h * j + i] = TD[w * i + j];
		}
	}

	 //沿垂直方向，每一列都做傅里叶反变换，一列有h个点
	for(i = 0; i < w; i++)
	{
		IFT(&TD[h * i], &FD[h * i], h); 
	}

	long size = 40 + 1024 + LineBytes * h; // 信息头+调色板 // 1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
    BITMAPINFO* lpDIB_IFT = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_IFT分配内存失败，则返回
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

	// 反变换完成，进行傅里叶变换标记
	Fourier_stage = 0;
	// 释放内存
	delete FD;
	delete TD;
	delete tempFD;
	free(lpBitsInfo);
    lpBitsInfo = lpDIB_IFT;
}

// 判断tempFD存在与否
bool Fast_FD_exist()
{
	if(tempFD && Fast_Fourier_stage)
		return true;
	else
		return false;
}

// 一维离散快速傅里叶变换
// 这里的count必须是2的幂,这里的r是指傅里叶变换点数的指数
void FFT(complex<double>* TD, complex<double>* FD, int r)
{
	int i,j,k;
	double angle;
	// 计算需要傅里叶变换的点数
	long count = 1 << r;
	// 分配内存
	complex<double>* W = new complex<double>[count / 2];

	// 计算加权系数
	for(i = 0; i < count / 2; i++)
	{
		angle = -2 * PI * i / count;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	// 每一层蝶形算法的输入与输出
	complex<double>* X1 = new complex<double>[count]; // 输入
	complex<double>* X2 = new complex<double>[count]; // 输出

	// 将时域空间的灰度值写入X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 蝶形算法
	int layer, group_size, times, index;
	// 暂时变量
	complex<double>* temp;
	// layer代表进行蝶形算法的层数
	for(layer = 0; layer < r; layer++)
	{
		// times代表当前层数需要进行的蝶形变换的次数，意象地可以理解为，蝴蝶的个数
		for(times = 0; times < 1 << layer; times++)
		{
			// group_size代表每次蝶形变换的规模大小，意象地可以理解为，蝴蝶的大小
			group_size = 1 << (r - layer);
			for(i = 0; i < group_size / 2; i++)
			{
				// index代表下标起点，例如：本层进行第二次蝶形变换，下标需要加上已经进行的蝶形变换次数 * 单次蝶形变换的规模大小
				index = times * group_size;
				X2[index + i] = X1[index + i] + X1[index + group_size / 2 + i];
				X2[index + group_size / 2 + i] = (X1[index + i] - X1[index + group_size / 2 + i]) * W[i * (1 << layer)];
			}
		}
		// 每一层计算结束之后，利用X1保存这一层X2得到的输出结果
		temp = X1;
		X1 = X2;
		X2 = temp;
	}

	// 重新排序（码位倒序排列）
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

	//释放内存
	delete W;
	delete X1;
	delete X2;
}

// 一维离散快速傅里叶反变换
void IFFT(complex<double>* FD, complex<double>* TD, int r)
{
	// 计算傅里叶反变换点数
	long count;
	count = 1 << r;

	// 分配运算所需存储器
	complex<double> * X = new complex<double>[count];

	// 将频域空间数据写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求频域空间数据的共轭复数
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域空间数据的共轭复数
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// 释放内存
	delete X;

}

// 快速傅里叶变换
void Fast_Fourier()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* TD = new complex<double>[FFT_w * FFT_h]; //空域空间
	complex<double>* FD = new complex<double>[FFT_w * FFT_h]; //频域空间

	int i,j;
    BYTE* pixel;
	int special_1 = 356;
	int special_2 = 206;

    // 多余的行和列全部置为0
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			if(i >= h || j >= w)
				TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
			else if( j == 200)// && i == 50 && i<= 200)//(j >= special_2 && j <= special_1 && i == 200)
			{
				// 访问图像灰度值
				pixel = lpBits + LineBytes * (h - 1 - i) + j;
				// TD[w * i + j] = complex<double>(*pixel, 0);  //赋予灰度值
				// 平移操作
				//TD[i * FFT_w + j] = complex<double>(*pixel * pow(-1, i + j), 0);
				TD[i * FFT_w + j] = complex<double>(0 * pow(-1, i + j), 0);
			}
			else
			{
				TD[i * FFT_w + j] = complex<double>(255 * pow(-1, i + j), 0);
			}
		}
	}

	// 沿水平方向作快速傅里叶变换
	for(i = 0; i < FFT_h; i++)
	{
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], width_index);
	}

	//沿垂直方向做傅里叶反变换时，因为像素点的灰度值不连续，有一个小技巧是做转置，并且利用TD的内存空间来存储转置的TD
	//转置
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			TD[FFT_h * j + i] = FD[FFT_w * i + j];
		}
	}

	// 沿垂直方向作快速傅里叶变换
	for(i = 0; i < FFT_w; i++)
	{
		FFT(&TD[FFT_h * i], &FD[FFT_h * i], height_index);
	}

	// 删除临时变量
	delete TD;

	// 生成频谱图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // 信息头+调色板 // 1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
    lpDIB_FFT = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_FFT分配内存失败，则返回
	if(NULL == lpDIB_FFT)
		return;

	// 复制原图像文件头和信息头
	memcpy(lpDIB_FFT, lpBitsInfo, 1064);

	// 改变原图像大小
	lpDIB_FFT->bmiHeader.biWidth = FFT_w;
	lpDIB_FFT->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_FFT->bmiColors[lpDIB_FFT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}
	// 用tempFD接收FD用于反变换
	tempFD = FD;
	
	// 如果存在反变换图像，则free掉
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	// 变换完成，进行傅里叶变换标记
	Fast_Fourier_stage = 1;
}

// 24位真彩图像快速傅里叶变换
void Color_Fast_Fourier()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* R_TD = new complex<double>[FFT_w * FFT_h]; //空域空间
	complex<double>* G_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* R_FD = new complex<double>[FFT_w * FFT_h]; //频域空间
	complex<double>* G_FD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_FD = new complex<double>[FFT_w * FFT_h];

	int i,j;
    BYTE* pixel;

    // 多余的行和列全部置为0
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
				// 访问图像RGB值
				pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
				// TD[w * i + j] = complex<double>(*pixel, 0);  //赋予灰度值
				// 平移操作
				B_TD[i * FFT_w + j] = complex<double>(*pixel * pow(-1, i + j), 0);
				G_TD[i * FFT_w + j] = complex<double>(*(pixel + 1) * pow(-1, i + j), 0);
				R_TD[i * FFT_w + j] = complex<double>(*(pixel + 2) * pow(-1, i + j), 0);
			}
		}
	}

	// 沿水平方向作快速傅里叶变换
	for(i = 0; i < FFT_h; i++)
	{
		FFT(&B_TD[FFT_w * i], &B_FD[FFT_w * i], width_index);
		FFT(&G_TD[FFT_w * i], &G_FD[FFT_w * i], width_index);
		FFT(&R_TD[FFT_w * i], &R_FD[FFT_w * i], width_index);
	}

	//沿垂直方向做傅里叶反变换时，因为像素点的灰度值不连续，有一个小技巧是做转置，并且利用TD的内存空间来存储转置的TD
	//转置
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			B_TD[FFT_h * j + i] = B_FD[FFT_w * i + j];
			G_TD[FFT_h * j + i] = G_FD[FFT_w * i + j];
			R_TD[FFT_h * j + i] = R_FD[FFT_w * i + j];
		}
	}

	// 沿垂直方向作快速傅里叶变换
	for(i = 0; i < FFT_w; i++)
	{
		FFT(&B_TD[FFT_h * i], &B_FD[FFT_h * i], height_index);
		FFT(&G_TD[FFT_h * i], &G_FD[FFT_h * i], height_index);
		FFT(&R_TD[FFT_h * i], &R_FD[FFT_h * i], height_index);
	}

	// 删除临时变量
	delete B_TD;
	delete G_TD;
	delete R_TD;

	// 生成频谱图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // 信息头 24真彩没有调色板
    lpDIB_FFT = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_FFT分配内存失败，则返回
	if(NULL == lpDIB_FFT)
		return;

	// 复制原图像文件头和信息头
	memcpy(lpDIB_FFT, lpBitsInfo, 40);

	// 改变原图像大小
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
			B_temp = sqrt(B_FD[j * FFT_h + i].real() * B_FD[j * FFT_h + i].real() + B_FD[j * FFT_h + i].imag() * B_FD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
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
	// 用tempFD接收FD用于反变换
	R_tempFD = R_FD;
	G_tempFD = G_FD;
	B_tempFD = B_FD;
	
	// 如果存在反变换图像，则free掉
	if(lpDIB_IFFT)
	{
		free(lpDIB_IFFT);
		lpDIB_IFFT = NULL;
	}

	// 变换完成，进行傅里叶变换标记
	Fast_Fourier_stage = 1;
}

// 快速傅里叶反变换
void IFast_Fourier()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* TD = new complex<double>[FFT_w * FFT_h]; //空域空间
	complex<double>* FD = new complex<double>[FFT_w * FFT_h]; //频域空间

	int i,j;
    BYTE* pixel;

   // 由于傅立叶变换发生了转置，故而需要转置赋值FD[i * FFT_w + j] = tempFD[j * FFT_h + i];
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			FD[i * FFT_w + j] = tempFD[j * FFT_h + i];
		}
	}

	// 沿水平方向作快速傅里叶反变换
	for(i = 0; i < FFT_h; i++)
	{
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], width_index);
	}

	//沿垂直方向做傅里叶反变换时，因为像素点的灰度值不连续，有一个小技巧是做转置，并且利用FD的内存空间来存储转置的TD
	//转置
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			FD[FFT_h * j + i] = TD[FFT_w * i + j];
		}
	}

	// 沿垂直方向作快速傅里叶反变换
	for(i = 0; i < FFT_w; i++)
	{
		IFFT(&FD[FFT_h * i], &TD[FFT_h * i], height_index);
	}

	// 生成原图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // 信息头+调色板 // 1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
    lpDIB_IFFT = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_FFT分配内存失败，则返回
	if(NULL == lpDIB_IFFT)
		return;
    
	// 复制原图像文件头和信息头
	memcpy(lpDIB_IFFT, lpBitsInfo, 1064);

	// 改变原图像大小
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

	// 变换完成，进行傅里叶变换标记
	Fast_Fourier_stage = 0;
	// 释放内存
	delete TD;
	delete FD;
	delete tempFD;
}

// 24位真彩图像快速傅里叶反变换
void Color_IFast_Fourier()
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	complex<double>* R_TD = new complex<double>[FFT_w * FFT_h]; //空域空间
	complex<double>* G_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* R_FD = new complex<double>[FFT_w * FFT_h]; //频域空间
	complex<double>* G_FD = new complex<double>[FFT_w * FFT_h];
	complex<double>* B_FD = new complex<double>[FFT_w * FFT_h];


	int i,j;
    BYTE* pixel;

   // 由于傅立叶变换发生了转置，故而需要转置赋值FD[i * FFT_w + j] = tempFD[j * FFT_h + i];
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			B_FD[i * FFT_w + j] = B_tempFD[j * FFT_h + i];
			G_FD[i * FFT_w + j] = G_tempFD[j * FFT_h + i];
			R_FD[i * FFT_w + j] = R_tempFD[j * FFT_h + i];
		}
	}

	// 沿水平方向作快速傅里叶反变换
	for(i = 0; i < FFT_h; i++)
	{
		IFFT(&B_FD[FFT_w * i], &B_TD[FFT_w * i], width_index);
		IFFT(&G_FD[FFT_w * i], &G_TD[FFT_w * i], width_index);
		IFFT(&R_FD[FFT_w * i], &R_TD[FFT_w * i], width_index);
	}

	//沿垂直方向做傅里叶反变换时，因为像素点的灰度值不连续，有一个小技巧是做转置，并且利用FD的内存空间来存储转置的TD
	//转置
    for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			B_FD[FFT_h * j + i] = B_TD[FFT_w * i + j];
			G_FD[FFT_h * j + i] = G_TD[FFT_w * i + j];
			R_FD[FFT_h * j + i] = R_TD[FFT_w * i + j];
		}
	}

	// 沿垂直方向作快速傅里叶反变换
	for(i = 0; i < FFT_w; i++)
	{
		IFFT(&B_FD[FFT_h * i], &B_TD[FFT_h * i], height_index);
		IFFT(&G_FD[FFT_h * i], &G_TD[FFT_h * i], height_index);
		IFFT(&R_FD[FFT_h * i], &R_TD[FFT_h * i], height_index);
	}

	// 生成原图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // 信息头 24位真彩图像没有信息头
    lpDIB_IFFT = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_FFT分配内存失败，则返回
	if(NULL == lpDIB_IFFT)
		return;
    
	// 复制原图像文件头和信息头
	memcpy(lpDIB_IFFT, lpBitsInfo, 40);

	// 改变原图像大小
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

	// 变换完成，进行傅里叶变换标记
	Fast_Fourier_stage = 0;
	// 释放内存
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

// 清除当前客户区
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

//-------------模板函数---------------//
void Template(int* Array, float coef)
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 为新图像分配内存
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;

	// 分配内存失败则返回
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// 复制BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// 找到新图像像素起始位置
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l;
	BYTE *pixel, *new_pixel;
	float result;
	// 由于使用了一个3x3的模板，故而不考虑第一行和最后一行以及第一列和最后一列
	// 行(除去边缘几行)
	for(i = 1; i < h - 1; i++)
	{
		// 列(除去边缘几列)
		for(j = 1; j < w - 1; j++)
		{
			// 指向新图第i行，第j个像素的指针
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			result = 0;
			// 计算3x3模板内像素灰度值的和
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// 指向原图在模板内每个像素点的灰度值，第i - 1 + k行，第j - 1 + l个像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					// 灰度值乘上模板系数后累加
					result += (*pixel) * Array[k * 3 + l];
				}
			}
			// 乘上外层系数
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

//-------------24位真彩模板函数---------------//
void Color_Template(int* Array, float coef)
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 为新图像分配内存
	LONG size = sizeof(BITMAPINFOHEADER) + h * LineBytes; //24位真彩图像没有调色板

	// 分配内存失败则返回
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// 复制BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// 找到新图像像素起始位置
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l;
	BYTE *pixel, *new_pixel;
	float R_result;
	float G_result;
	float B_result;
	// 由于使用了一个3x3的模板，故而不考虑第一行和最后一行以及第一列和最后一列
	// 行(除去边缘几行)
	for(i = 1; i < h - 1; i++)
	{
		// 列(除去边缘几列)
		for(j = 1; j < w - 1; j++)
		{
			// 指向新图第i行，第j个像素的指针
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j * 3;
			R_result = 0;
			G_result = 0;
			B_result = 0;
			// 计算3x3模板内像素灰度值的和
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// 指向原图在模板内每个像素点的灰度值，第i - 1 + k行，第j - 1 + l个像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + (j - 1 + l) * 3;
					// 灰度值乘上模板系数后累加
					B_result += (*pixel) * Array[k * 3 + l];
					G_result += (*(pixel + 1)) * Array[k * 3 + l];
					R_result += (*(pixel + 2)) * Array[k * 3 + l];
				}
			}
			// 乘上外层系数
			R_result *= coef;
			G_result *= coef;
			B_result *= coef;
			// blue
			if (B_result < 0)
				*new_pixel = 0;
			else if(B_result > 255)
				*new_pixel = 255;
			else
				*new_pixel = (BYTE) (B_result + 0.5); //+0.5四舍五入
			// green
			if (G_result < 0)
				*(new_pixel + 1) = 0;
			else if(G_result > 255)
				*(new_pixel + 1) = 255;
			else
				*(new_pixel + 1) = (BYTE) (G_result + 0.5); //+0.5四舍五入
			// red
			if (R_result < 0)
				*(new_pixel + 2) = 0;
			else if(R_result > 255)
				*(new_pixel + 2) = 255;
			else
				*(new_pixel + 2) = (BYTE) (R_result + 0.5); //+0.5四舍五入
		}
	}
}


// 均值滤波
void AverageFilter()
{
	int Array[9]; //3x3模板
	//标准均值滤波
	Array[0] = 1;	Array[1] = 1;	Array[2] = 1;
	Array[3] = 1;	Array[4] = 1;	Array[5] = 1;
	Array[6] = 1;	Array[7] = 1;	Array[8] = 1;
	Template(Array, (float)1/9);
}

// 24位真彩图像均值滤波
void Color_AverageFilter()
{
	int Array[9]; //3x3模板
	//标准均值滤波
	Array[0] = 1;	Array[1] = 1;	Array[2] = 1;
	Array[3] = 1;	Array[4] = 1;	Array[5] = 1;
	Array[6] = 1;	Array[7] = 1;	Array[8] = 1;
	Color_Template(Array, (float)1/9);
}

// 高斯滤波
void GaussFilter()
{
	int Array[9]; //3x3模板
	//高斯滤波模板
	Array[0] = 1;	Array[1] = 2;	Array[2] = 1;
	Array[3] = 2;	Array[4] = 4;	Array[5] = 2;
	Array[6] = 1;	Array[7] = 2;	Array[8] = 1;
	Template(Array, (float)1/16);
}

// 24位真彩高斯滤波
void Color_GaussFilter()
{
	int Array[9]; //3x3模板
	//高斯滤波模板
	Array[0] = 1;	Array[1] = 2;	Array[2] = 1;
	Array[3] = 2;	Array[4] = 4;	Array[5] = 2;
	Array[6] = 1;	Array[7] = 2;	Array[8] = 1;
	Color_Template(Array, (float)1/16);
}


//-------------中值滤波函数---------------//
BYTE WINAPI GetMediumNum(BYTE* Array, int Array_size)
{
	int i,j;
	BYTE temp;
	// 用冒泡法对数组进行排序
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
	// 返回中值
	return Array[(Array_size - 1) / 2];
}


// 中值滤波
void MediumFilter()
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 为新图像分配内存
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;

	// 如果内存分配失败则返回
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// 复制BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	// 找到新图像像素起始位置
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, Array_width, Array_height, Array_size;
	BYTE *pixel, *new_pixel;
	BYTE Value[9]; //3x3模板
	Array_width = 3; //模板的宽
	Array_height = 3; //模板的高
	Array_size = Array_width * Array_height; //记录模板大小
	// 行(除去边缘几行)
	for(i = 1; i < h - 1; i++)
	{
		// 列(除去边缘几列)
		for(j = 1; j < w - 1; j++)
		{
			// 指向新图第i行，第j个像素的指针
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			// 计算 3x3模板内像素的灰度值
			for (k = 0; k < Array_height; k++)
			{
				for (l = 0; l < Array_width; l++)
				{
					// 指向原图第i - 1 + k行，第j - 1 + l列像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					// 保存像素值
					Value[k * Array_height + l] = *pixel;
				}
			}
			// 获取中值
			*new_pixel = GetMediumNum(Value, Array_size);
		}
	}
}


// 24位真彩图像中值滤波
void Color_MediumFilter()
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 为新图像分配内存
	LONG size = sizeof(BITMAPINFOHEADER) + h * LineBytes; //24位图像没有调色板

	// 如果内存分配失败则返回
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// 复制BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	// 找到新图像像素起始位置
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, Array_width, Array_height, Array_size;
	BYTE *pixel, *new_pixel;
	BYTE R_Value[9]; //3x3模板
	BYTE G_Value[9]; //3x3模板
	BYTE B_Value[9]; //3x3模板
	Array_width = 3; //模板的宽
	Array_height = 3; //模板的高
	Array_size = Array_width * Array_height; //记录模板大小
	// 行(除去边缘几行)
	for(i = 1; i < h - 1; i++)
	{
		// 列(除去边缘几列)
		for(j = 1; j < w - 1; j++)
		{
			// 指向新图第i行，第j个像素的指针
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j * 3;
			// 计算 3x3模板内像素的灰度值
			for (k = 0; k < Array_height; k++)
			{
				for (l = 0; l < Array_width; l++)
				{
					// 指向原图第i - 1 + k行，第j - 1 + l列像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + (j - 1 + l) * 3;
					// 保存像素值
					B_Value[k * Array_height + l] = *pixel;
					G_Value[k * Array_height + l] = *(pixel + 1);
					R_Value[k * Array_height + 2] = *(pixel + 2);
				}
			}
			// 获取中值
			*new_pixel = GetMediumNum(B_Value, Array_size);
			*(new_pixel + 1) = GetMediumNum(G_Value, Array_size);
			*(new_pixel + 2) = GetMediumNum(R_Value, Array_size);
		}
	}
}


// 拉普拉斯锐化
void LaplaceSharpening()
{
	int Array[9]; //3x3模板
/*
	//拉普拉斯算子
	Array[0] = 0;	Array[1] = 1;	Array[2] = 0;
	Array[3] = 1;	Array[4] = -4;	Array[5] = 1;
	Array[6] = 0;	Array[7] = 1;	Array[8] = 0;
*/	
	//拉普拉斯锐化
	Array[0] = -1;	Array[1] = -1;	Array[2] = -1;
	Array[3] = -1;	Array[4] = 9;	Array[5] = -1;
	Array[6] = -1;	Array[7] = -1;	Array[8] = -1;
	Template(Array, (float)1);
}

// 24位真彩图像拉普拉斯锐化
void Color_LaplaceSharpening()
{
	int Array[9]; //3x3模板
/*
	//拉普拉斯算子
	Array[0] = 0;	Array[1] = 1;	Array[2] = 0;
	Array[3] = 1;	Array[4] = -4;	Array[5] = 1;
	Array[6] = 0;	Array[7] = 1;	Array[8] = 0;
*/	
	//拉普拉斯锐化
	Array[0] = -1;	Array[1] = -1;	Array[2] = -1;
	Array[3] = -1;	Array[4] = 9;	Array[5] = -1;
	Array[6] = -1;	Array[7] = -1;	Array[8] = -1;
	Color_Template(Array, (float)1);
}

// 8位灰度图像Robert锐化模板
void TemplateForRobert(int* Array_1, int* Array_2, float coef)
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 为新图像分配内存
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;

	// 分配内存失败则返回
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// 复制BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// 找到新图像像素起始位置
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, m, n;
	BYTE *pixel, *new_pixel;
	float result, result_1, result_2;
	// 由于使用了一个3x3的模板，故而不考虑第一行和最后一行以及第一列和最后一列
	// 行(除去边缘几行)
	for(i = 1; i < h - 1; i++)
	{
		// 列(除去边缘几列)
		for(j = 1; j < w - 1; j++)
		{
			// 指向新图第i行，第j个像素的指针
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			result = 0;
			result_1 = 0;
			result_2 = 0;
			// 计算3x3模板内像素灰度值的和
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// 指向原图在模板内每个像素点的灰度值，第i - 1 + k行，第j - 1 + l个像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					// 灰度值乘上模板系数后累加
					result_1 += (*pixel) * Array_1[k * 3 + l];
				}
			}

			for (m = 0; m < 3; m++)
			{
				for (n = 0; n < 3; n++)
				{
					// 指向原图在模板内每个像素点的灰度值，第i - 1 + k行，第j - 1 + l个像素的指针
					pixel = lpBits + LineBytes * (h - i - m) + j - 1 + n;
					// 灰度值乘上模板系数后累加
					result_2 += (*pixel) * Array_2[m * 3 + n];
				}
			}
			if(result_1 < 0)
			    result_1 = result_1 * (-1);
			if(result_2 < 0)
				result_2 = result_2 * (-1);

            result = result_1 + result_2;
			// 乘上外层系数
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

// 24位真彩图像Robert锐化模板
void Color_TemplateForRobert(int* Array_1, int* Array_2, float coef)
{
    if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// 为新图像分配内存
	LONG size = sizeof(BITMAPINFOHEADER) + h * LineBytes; // 24位真彩图像没有调色板

	// 分配内存失败则返回
	if (NULL == (new_lpBitsInfo = (LPBITMAPINFO) malloc(size)))
		return;

	// 复制BMP
	memcpy(new_lpBitsInfo, lpBitsInfo, size);

	// 找到新图像像素起始位置
	BYTE* lpNewBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l, m, n;
	BYTE *pixel, *new_pixel;
	float R_result, R_result_1, R_result_2;
	float G_result, G_result_1, G_result_2;
	float B_result, B_result_1, B_result_2;
	// 由于使用了一个3x3的模板，故而不考虑第一行和最后一行以及第一列和最后一列
	// 行(除去边缘几行)
	for(i = 1; i < h - 1; i++)
	{
		// 列(除去边缘几列)
		for(j = 1; j < w - 1; j++)
		{
			// 指向新图第i行，第j个像素的指针
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
			// 计算3x3模板内像素灰度值的和
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// 指向原图在模板内每个像素点的灰度值，第i - 1 + k行，第j - 1 + l个像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + (j - 1 + l) * 3;
					// 灰度值乘上模板系数后累加
					B_result_1 += (*pixel) * Array_1[k * 3 + l];
					G_result_1 += (*(pixel + 1)) * Array_1[k * 3 + l];
					R_result_1 += (*(pixel + 2)) * Array_1[k * 3 + l];
				}
			}

			for (m = 0; m < 3; m++)
			{
				for (n = 0; n < 3; n++)
				{
					// 指向原图在模板内每个像素点的灰度值，第i - 1 + k行，第j - 1 + l个像素的指针
					pixel = lpBits + LineBytes * (h - i - m) + (j - 1 + n) * 3;
					// 灰度值乘上模板系数后累加
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
			// 乘上外层系数
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

// 梯度锐化——Robert锐化
void GradSharpening()
{
	int Array_1[9]; //3x3模板
	int Array_2[9]; //3x3模板
	//使用梯度近似计算的另一种形式——Robert算子
	Array_1[0] = 0;	Array_1[1] = 0;	Array_1[2] = 0;
	Array_1[3] = 0;	Array_1[4] = 1;	Array_1[5] = 0;
	Array_1[6] = 0;	Array_1[7] = 0; Array_1[8] = -1;

	Array_2[0] = 0;	Array_2[1] = 0;	Array_2[2] = 0;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 1;
	Array_2[6] = 0;	Array_2[7] = -1; Array_2[8] = 0;

	TemplateForRobert(Array_1, Array_2, (float)1.1);
}

// 24位真彩图像
// 梯度锐化——Robert锐化
void Color_GradSharpening()
{
	int Array_1[9]; //3x3模板
	int Array_2[9]; //3x3模板
	//使用梯度近似计算的另一种形式——Robert算子
	Array_1[0] = 0;	Array_1[1] = 0;	Array_1[2] = 0;
	Array_1[3] = 0;	Array_1[4] = 1;	Array_1[5] = 0;
	Array_1[6] = 0;	Array_1[7] = 0; Array_1[8] = -1;

	Array_2[0] = 0;	Array_2[1] = 0;	Array_2[2] = 0;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 1;
	Array_2[6] = 0;	Array_2[7] = -1; Array_2[8] = 0;

	Color_TemplateForRobert(Array_1, Array_2, (float)1.1);
}

// 梯度锐化——Priwitt锐化
void PriwittSharpening()
{
	int Array_1[9]; //3x3模板
	int Array_2[9]; //3x3模板
	//使用梯度近似计算的另一种形式——Robert算子
	Array_1[0] = -1;	Array_1[1] = 0;	Array_1[2] = 1;
	Array_1[3] = -1;	Array_1[4] = 0;	Array_1[5] = 1;
	Array_1[6] = -1;	Array_1[7] = 0; Array_1[8] = 1;

	Array_2[0] = -1; Array_2[1] = -1; Array_2[2] = -1;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 0;
	Array_2[6] = 1;	Array_2[7] = 1; Array_2[8] = 1;

	TemplateForRobert(Array_1, Array_2, (float)1);
}

// 24位真彩图像
// 梯度锐化——Priwitt锐化
void Color_PriwittSharpening()
{
	int Array_1[9]; //3x3模板
	int Array_2[9]; //3x3模板
	//使用梯度近似计算的另一种形式——Robert算子
	Array_1[0] = -1;	Array_1[1] = 0;	Array_1[2] = 1;
	Array_1[3] = -1;	Array_1[4] = 0;	Array_1[5] = 1;
	Array_1[6] = -1;	Array_1[7] = 0; Array_1[8] = 1;

	Array_2[0] = -1; Array_2[1] = -1; Array_2[2] = -1;
	Array_2[3] = 0;	Array_2[4] = 0;	Array_2[5] = 0;
	Array_2[6] = 1;	Array_2[7] = 1; Array_2[8] = 1;

	Color_TemplateForRobert(Array_1, Array_2, (float)1);
}

//----------------理想频域滤波函数---------------//
//choose=0 低通 1 高通 
void IdealFilter(int choose)
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	// 图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	    
	// 获取图像中心
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // 滤波半径的平方
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	} 
    else
	{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// 滤波
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
	        		tempFD[i + FFT_h * j] *= 0; //保留低频部分使图像可看
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
	// 生成频谱图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // 信息头+调色板 // 1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
    BITMAPINFO* lpDIB_Ideal = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_Ideal分配内存失败，则返回
	if(NULL == lpDIB_Ideal)
		return;

	// 复制原图像文件头和信息头
	memcpy(lpDIB_Ideal, lpBitsInfo, 1064);

	// 改变原图像大小
	lpDIB_Ideal->bmiHeader.biWidth = FFT_w;
	lpDIB_Ideal->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_Ideal->bmiColors[lpDIB_Ideal->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			temp = sqrt(tempFD[j * FFT_h + i].real() * tempFD[j * FFT_h + i].real() + tempFD[j * FFT_h + i].imag() * tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}
	// 在原来的频谱图像上显示
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Ideal;
	}
}

// 24位真彩图像
//----------------理想频域滤波函数---------------//
//choose=0 低通 1 高通 
void Color_IdealFilter(int choose)
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	// 图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}

	    
	// 获取图像中心
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // 滤波半径的平方
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	} 
    else
	{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// 滤波
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
	        		R_tempFD[i + FFT_h * j] *= 0; //保留低频部分使图像可看
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
	// 生成频谱图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // 信息头 24位真彩图像没有调色板
    BITMAPINFO* lpDIB_Ideal = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_Ideal分配内存失败，则返回
	if(NULL == lpDIB_Ideal)
		return;

	// 复制原图像文件头和信息头
	memcpy(lpDIB_Ideal, lpBitsInfo, 40);

	// 改变原图像大小
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
	    	B_temp = sqrt(B_tempFD[j * FFT_h + i].real() * B_tempFD[j * FFT_h + i].real() + B_tempFD[j * FFT_h + i].imag() * B_tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(B_temp > 255)
			    B_temp = 255;
            *pixel = (BYTE)B_temp;

			// green
	    	G_temp = sqrt(G_tempFD[j * FFT_h + i].real() * G_tempFD[j * FFT_h + i].real() + G_tempFD[j * FFT_h + i].imag() * G_tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(G_temp > 255)
			    G_temp = 255;
            *(pixel + 1) = (BYTE)G_temp;

			// red
	    	R_temp = sqrt(R_tempFD[j * FFT_h + i].real() * R_tempFD[j * FFT_h + i].real() + R_tempFD[j * FFT_h + i].imag() * R_tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(R_temp > 255)
			    R_temp = 255;
            *(pixel + 2) = (BYTE)R_temp;
		}
	}
	// 在原来的频谱图像上显示
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Ideal;
	}
}

//-------------巴特沃斯频域滤波函数--------------//
//choose=0 低通 1 高通, n为阶数
void ButterworthFilter(int choose)
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}
    
	// 阶数
	int n=1;

	// 获取图像中心
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // 滤波半径的平方
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	}
	else{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// 滤波
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
	// 生成频谱图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + 1024 + newLineBytes * FFT_h; // 信息头+调色板 // 1024是由256位色彩*4得到(RGB占三个字节，Reserved占用一个字节)
    BITMAPINFO* lpDIB_Butterworth = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_Butterworth分配内存失败，则返回
	if(NULL == lpDIB_Butterworth)
		return;

	// 复制原图像文件头和信息头
	memcpy(lpDIB_Butterworth, lpBitsInfo, 1064);

	// 改变原图像大小
	lpDIB_Butterworth->bmiHeader.biWidth = FFT_w;
	lpDIB_Butterworth->bmiHeader.biHeight = FFT_h;

	lpBits = (BYTE*)&lpDIB_Butterworth->bmiColors[lpDIB_Butterworth->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++)
	{
        for(j = 0; j < FFT_w; j++)
		{
			pixel = lpBits + newLineBytes * (FFT_h - 1 - i) + j;
			temp = sqrt(tempFD[j * FFT_h + i].real() * tempFD[j * FFT_h + i].real() + tempFD[j * FFT_h + i].imag() * tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(temp > 255)
			    temp = 255;
            *pixel = (BYTE)temp;
		}
	}
	// 在原来的频谱图像上显示
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Butterworth;
	}
}

// 24位真彩图像
//-------------巴特沃斯频域滤波函数--------------//
//choose=0 低通 1 高通, n为阶数
void Color_ButterworthFilter(int choose)
{
	if(NULL == lpBitsInfo)// 图像未加载，返回
		return;

	//图像读取四部曲
    int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂指数，即进行蝶形变换的层数
	int width_index = 0;
	while(FFT_w * 2 <= w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// 如果读取的图像宽不是2的整数次方，则需要再乘上2作为FFT_w，超出原来宽度的部分填0
	if(FFT_w != w)
	{
		FFT_w *= 2;
		width_index++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂指数，即进行蝶形变换的层数
	int height_index = 0;
	while(FFT_h * 2 <= h)
	{
		FFT_h *= 2;
		height_index++;
	}

	// 如果读取的图像高不是2的整数次方，则需要再乘上2作为FFT_h，超出原来高度的部分填0
	if(FFT_h != h)
	{
		FFT_h *= 2;
		height_index++;
	}
    
	// 阶数
	int n=1;

	// 获取图像中心
	int centerWidth = FFT_w / 2;
	int centerHeight = FFT_h / 2;

	double r2; // 滤波半径的平方
	if(FFT_w > FFT_h)
	{
		r2 = pow(FFT_h - centerWidth, 2) * 2;
	}
	else{
		r2 = pow(FFT_h - centerHeight, 2) * 2;
	}

	// 滤波
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
	// 生成频谱图像
	int newLineBytes = (FFT_w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	long size = 40 + newLineBytes * FFT_h; // 信息头 24位真彩图像没有调色板
    BITMAPINFO* lpDIB_Butterworth = (LPBITMAPINFO)malloc(size);

	// 添加保护，如果lpDIB_Butterworth分配内存失败，则返回
	if(NULL == lpDIB_Butterworth)
		return;
	
	// 复制原图像文件头和信息头
	memcpy(lpDIB_Butterworth, lpBitsInfo, 40);

	// 改变原图像大小
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
			R_temp = sqrt(R_tempFD[j * FFT_h + i].real() * R_tempFD[j * FFT_h + i].real() + R_tempFD[j * FFT_h + i].imag() * R_tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(R_temp > 255)
			    R_temp = 255;
            *(pixel + 2) = (BYTE)R_temp;

			G_temp = sqrt(G_tempFD[j * FFT_h + i].real() * G_tempFD[j * FFT_h + i].real() + G_tempFD[j * FFT_h + i].imag() * G_tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(G_temp > 255)
			    G_temp = 255;
            *(pixel + 1) = (BYTE)G_temp;

			B_temp = sqrt(B_tempFD[j * FFT_h + i].real() * B_tempFD[j * FFT_h + i].real() + B_tempFD[j * FFT_h + i].imag() * B_tempFD[j * FFT_h + i].imag()) * 2000; // 增强，乘以两千
            if(B_temp > 255)
			    B_temp = 255;
            *pixel = (BYTE)B_temp;
		}
	}
	// 在原来的频谱图像上显示
	if(lpDIB_FFT)
	{
		free(lpDIB_FFT);
		lpDIB_FFT = lpDIB_Butterworth;
	}
}
