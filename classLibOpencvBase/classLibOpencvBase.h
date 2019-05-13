// classLibOpencvBase.h

#pragma once

#include "opencv2/opencv.hpp"
using namespace cv;

#if  (defined WIN32 || defined _WIN32 || defined WINCE  ) &&  defined MYDLL_EXPORTS 
#define  OPENCV_WARP_API_EXPORTS  _declspec(dllexport)
#else
#define  OPENCV_WARP_API_EXPORTS 
#endif 

namespace TestCppDll
{
	extern "C" __declspec(dllexport) int __stdcall add_lk( int a, int b   );
}

OPENCV_WARP_API_EXPORTS  class  CNaiveCpp 
{
public: 
	CNaiveCpp()
	{
	}
	~CNaiveCpp()
	{
	}
public:

public:
#ifdef  MFC__FRAME
	//  数据到bmp的长度转换
	int cal_WIDTHBYTES(  int   bits  ,int channels )
	{
		if( 3 == channels )
		{
			//下面的这个式子是对的，但应该理解清楚.
			// 这个适用于bgr图像的。
			return   ((( bits)+ 31 )/32*4  )    ;
		}
		else if( 1 ==  channels )
		{
			//  这里应该是多少呢？好好算算吧.下面的式子是不对的。
			return   ((( bits)+ 7 )/8   )    ;
		}
	}

	void procImageShowAccordingToWH(   int w,int h,  cv::Mat & Image )
	{
		double rateW = double(w)/Image.cols;
		double rateH = double(h)/Image.rows;
		double rateF = rateW>rateH? rateH:rateW;
		cv::Mat imgSc;
		resize(  Image,imgSc, Size(  Image.cols*  rateF,  Image.rows*rateF ));
		Image = imgSc;
	}


	void  DrawIplImage2MFC( Mat src , unsigned int id)
	{	
		if( 3> src.channels() )
		{
			//appendTextToTextEdit( IDC_EDIT1, CString("<info:图像显示：>单通道图像显示部分还没完成。")  );
			//appendTextToTextEdit( IDC_EDIT1, CString("<info:图像显示：>所以对于非彩色图像，暂时转换为三通道图像来显示... ...  ")  );
			// 改为三通道图像 
			Mat srcBGR;
			cvtColor( src,  srcBGR,  CV_GRAY2BGR );
			srcBGR.copyTo( src );
		}

		//  src -- iamgecontrl-- bmp 
		BYTE *g_pBits;
		HDC g_hMemDC;
		HBITMAP g_hBmp, g_hOldBmp;
		CDC *pDC;
		CStatic *pic;
		int width, height;
		CRect rect;

		//  控件的信息
		pDC = GetDlgItem(id)->GetDC();
		pic = (CStatic*)GetDlgItem(id);
		pic->GetClientRect(&rect);
		width = rect.Width();
		height = rect.Height();
		if( 1 )
		{
			if( 0 )
			{
				Mat srcR;
				resize(  src, srcR, Size( width, height ) );
				srcR.copyTo( src );
			}


			procImageShowAccordingToWH(  width,height,  src );
		}
		if( src.cols %4!= 0 ||  src.rows %4 != 0 )
		{
			Mat tmp;
			resize(src, tmp , Size( ((int)src.cols/4+1 )*4, ((int)src.rows/4+1 )*4    ) );
			tmp.copyTo(  src );
		}

		//  bmp图片的信息
		IplImage *img = & IplImage( src );
		g_hMemDC =::CreateCompatibleDC(pDC->m_hDC);
		BYTE bmibuf[sizeof(BITMAPINFO)+256 * sizeof(RGBQUAD)];
		memset(bmibuf, 0, sizeof(bmibuf));
		BITMAPINFO *pbmi = (BITMAPINFO*)bmibuf;
		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbmi->bmiHeader.biWidth = img->width;
		pbmi->bmiHeader.biHeight = img->height;
		pbmi->bmiHeader.biPlanes = 1;
		pbmi->bmiHeader.biBitCount = 24;
		pbmi->bmiHeader.biCompression= BI_RGB;  //BI_RLE8
		g_hBmp =::CreateDIBSection(g_hMemDC, pbmi, DIB_RGB_COLORS, (void**)&g_pBits, 0, 0);
		g_hOldBmp = (HBITMAP)::SelectObject(g_hMemDC, g_hBmp);
		BitBlt(g_hMemDC, 0, 0, width, height, pDC->m_hDC, 0, 0, SRCCOPY);
		if( 1 )
		{
			//int l_width = WIDTHBYTES( img->width* pbmi->bmiHeader.biBitCount );
			int l_width = cal_WIDTHBYTES( img->width* pbmi->bmiHeader.biBitCount , src.channels()  );

			for (int row = 0; row < img->height;row++)  
				memcpy( &g_pBits[row*l_width],  &img->imageData[ (img->height - row - 1)*l_width],  l_width );
			TransparentBlt(pDC->m_hDC, 0, 0, width, height, g_hMemDC, 0, 0,
				img->width, img->height, RGB(0, 0, 0));
			SelectObject(g_hMemDC, g_hOldBmp);
		}

		//另一种显示方式
		if( 1 )
		{
			//id = IDC_STATIC2;
			CBitmap  bm;
			bm.Attach(  g_hBmp );
			CStatic *p = (CStatic* )(GetDlgItem( id ) );
			p->ModifyStyle( 0xf,SS_BITMAP |SS_CENTERIMAGE | SS_REALSIZEIMAGE  );
			p->SetBitmap(  bm );
		}

		//release 
		DeleteDC(g_hMemDC);
		DeleteObject(pic);
		DeleteObject(g_hBmp);
		DeleteObject(g_hOldBmp);
	}

	// mfc画图
	void  DrawIplImage2MFC_loadBmp( Mat src , unsigned int id)
	{
		CStatic *p = (CStatic* )(GetDlgItem( id ) );
		CRect rect;
		p->GetClientRect(&rect);
		int width = rect.Width();
		int height = rect.Height();

		//resize 
		Mat srcR;
		resize(  src, srcR, Size( width, height ) );
		srcR.copyTo( src );

		const   char*  nameWindow =   "tmp.bmp" ;
		imwrite(  nameWindow,  src );
		HBITMAP  bm = (  HBITMAP)::LoadImageW(NULL, _T(  "tmp.bmp" ),IMAGE_BITMAP,
			0, 0, LR_CREATEDIBSECTION| LR_DEFAULTSIZE |LR_LOADFROMFILE  ) ;
		//CBitMap  bm; bm.LoadBitmapW(  IDB_BITMAP1 );  ( 这个是另外的一种加载bmp的方式)
		p->ModifyStyle( 0xf,SS_BITMAP |SS_CENTERIMAGE | SS_REALSIZEIMAGE  );
		p->SetBitmap(  bm );
	}


	void fetchFileName(string & nameOut)
	{
		CString picPath;   //定义图片路径变量  
		CFileDialog dlg(TRUE, NULL, NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
			NULL, this);   //选择文件对话框  
		//const char *nameOfDlg = "open image file";
		//CFileDialog dlg(TRUE, NULL , nameOfDlg,
		//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
		//	NULL, this);   //选择文件对话框  
		if (dlg.DoModal() == IDOK)
		{
			picPath = dlg.GetPathName();  //获取图片路径  
		}
		else
		{
			return;
		}
		if (picPath.IsEmpty())
			return;
		////CString to string  使用这个方法记得字符集选用“使用多字节字符”，不然会报错  
		//string picpath = picPath.GetBuffer(0);

		//从这里开始进行转化，这是一个宏定义
		USES_CONVERSION;
		//进行转换
		char* keyChar = T2A(picPath.GetBuffer(0));
		picPath.ReleaseBuffer();
		//Mat testMat= imread( keyChar ) ;
		//if( !testMat.data )
		//	keyChar = NULL;
		string picpath(keyChar);
		nameOut = picpath;
	}
#endif 

	int  denoise()
	{
		return 0;
	}

	int  getA()
	{
		string nameOfFile = "./test.jpg";
		//fetchFileName(  nameOfFile );
cv:Mat src = imread( nameOfFile );
		if( src.data )
		{
			imshow( "src",  src);
			waitKey();
		}
		return 10;
	}
};



#ifdef _MANAGED
using namespace System;
#endif

namespace classLibOpencvBase {

#ifdef _MANAGED
	public  ref    class  LkProccesion

#else
	class  LkProccesion
#endif
	{
		// TODO: 在此处添加此类的方法。
	public:
		LkProccesion()
		{

		}
		~LkProccesion()
		{

		}
	public:
		int  getB()
		{
			CNaiveCpp   ca;
			return ca.getA();
		}


	};
};


