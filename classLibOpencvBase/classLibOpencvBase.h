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
	//  ���ݵ�bmp�ĳ���ת��
	int cal_WIDTHBYTES(  int   bits  ,int channels )
	{
		if( 3 == channels )
		{
			//��������ʽ���ǶԵģ���Ӧ��������.
			// ���������bgrͼ��ġ�
			return   ((( bits)+ 31 )/32*4  )    ;
		}
		else if( 1 ==  channels )
		{
			//  ����Ӧ���Ƕ����أ��ú������.�����ʽ���ǲ��Եġ�
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
			//appendTextToTextEdit( IDC_EDIT1, CString("<info:ͼ����ʾ��>��ͨ��ͼ����ʾ���ֻ�û��ɡ�")  );
			//appendTextToTextEdit( IDC_EDIT1, CString("<info:ͼ����ʾ��>���Զ��ڷǲ�ɫͼ����ʱת��Ϊ��ͨ��ͼ������ʾ... ...  ")  );
			// ��Ϊ��ͨ��ͼ�� 
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

		//  �ؼ�����Ϣ
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

		//  bmpͼƬ����Ϣ
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

		//��һ����ʾ��ʽ
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

	// mfc��ͼ
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
		//CBitMap  bm; bm.LoadBitmapW(  IDB_BITMAP1 );  ( ����������һ�ּ���bmp�ķ�ʽ)
		p->ModifyStyle( 0xf,SS_BITMAP |SS_CENTERIMAGE | SS_REALSIZEIMAGE  );
		p->SetBitmap(  bm );
	}


	void fetchFileName(string & nameOut)
	{
		CString picPath;   //����ͼƬ·������  
		CFileDialog dlg(TRUE, NULL, NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
			NULL, this);   //ѡ���ļ��Ի���  
		//const char *nameOfDlg = "open image file";
		//CFileDialog dlg(TRUE, NULL , nameOfDlg,
		//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
		//	NULL, this);   //ѡ���ļ��Ի���  
		if (dlg.DoModal() == IDOK)
		{
			picPath = dlg.GetPathName();  //��ȡͼƬ·��  
		}
		else
		{
			return;
		}
		if (picPath.IsEmpty())
			return;
		////CString to string  ʹ����������ǵ��ַ���ѡ�á�ʹ�ö��ֽ��ַ�������Ȼ�ᱨ��  
		//string picpath = picPath.GetBuffer(0);

		//�����￪ʼ����ת��������һ���궨��
		USES_CONVERSION;
		//����ת��
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
		// TODO: �ڴ˴���Ӵ���ķ�����
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


