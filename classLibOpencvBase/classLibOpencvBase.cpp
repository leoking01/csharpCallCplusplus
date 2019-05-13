// 这是主 DLL 文件。

#include "stdafx.h"

#include "classLibOpencvBase.h"

#include  <opencv2/opencv.hpp>
using namespace  cv;

namespace TestCppDll
{
	__declspec(dllexport)   int  __stdcall  add_lk( int a, int b   )
	{
		//Mat src = imread( "D:\\imges/star.jpeg",  1   );
		//imshow( "src", src );
		//waitKey( );

		string nameOfFile = "./test.jpg";
		//fetchFileName(  nameOfFile );
cv:Mat src = imread( nameOfFile );
		if( src.data )
		{
			imshow( "srcB",  src);
			waitKey();
		}


		return a+ b ;
	}

}