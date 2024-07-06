/*===========================================================================
  This demonstrative example is provided by the teaching assistant,
  Mr. Shih-Hung Liao (廖世宏), and modified by the instructor, Prof. Lan.

  (1) It can be compiled and executed correctly under the DEV-C++, and Visual C++
      environments.
  (2) In order to run this program, you should also have the "bmp.h" and
      "bmp.cpp" files installed in your current directory or whichever directory
      the C++ compiler is directed to search for.
  (3) The DEV-C++ is a free C++ development environment that is recommended for
      this course. Refer to http://www.bloodshed.net/dev/devcpp.html.

                             Apr. 3, 2006
============================================================================*/
#include <iostream>
#include <stdlib.h>
#include "bmp.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; // MaxBMPSizeX and MaxBMPSizeY are defined in "bmp.h"
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;
	float u=0,v=0;
	int a1, b1, c1, d1;
	float aval, bval, cval, dval;

	int xoffset =  width  / 2;
	int yoffset =  height / 2;
	// 開啟並讀取全彩(24bits)bmp 影像圖檔
	// RGB 三個 channel 的值存入 R, G, B 三個自訂的陣列中
	// 陣列大小不得大於 1024x1024 (目前設定) 
	// 若需要更大的影像size，可將bmp.h中的 MaxBMPSizeX 與 MaxBMPSizeY 重新設定 
	//open_bmp("lena.bmp", R, G, B, width, height); //for true color images
	open_bmp("lena.bmp", R, R, R, width, height); // for gray images
	// The starting pixel of a bmp image is at the bottom-left corner, which has 
	// coordinates x=0 and y=0.  The last pixel is at the top-right corner.

    /*----------------------------------------------------------------------------
	      範例：產生數位負片（請將主程式放在此處）
    ----------------------------------------------------------------------------*/
	for (j = 0; j < width; j++){ // typical image scanning order
	   for (i = 0; i < width; i++){ 

		u=(int)(0.703*i+0.642*j)/(-0.0005*i+0.012*j+1);
		v=(int)(0.074*i+1.2038*j)/(-0.0005*i+0.012*j+1);
		r[i][j] = R[u][v];
		/*a1 = (int)(u + 1);
		b1 = (int)(u);
		c1 = (int)(v + 1);
		d1 = (int)(v);
		aval = (float)((width - i) / width);
		bval = (float)(1 - aval);
		cval = (float)(j / height);
		dval = (float)(1 - cval);

		r[i+ xoffset][j+ yoffset] =R[b1][c1] * (aval)*(dval)+
								   R[a1][c1] * (bval)*(dval)+
								   R[b1][d1] * (aval)*(cval)+
								   R[a1][d1] * (bval)*(cval);
		g[i + xoffset][j + yoffset] = G[b1][c1] * (aval)*(dval)+
										  G[a1][c1] * (bval)*(dval)+
										  G[b1][d1] * (aval)* (cval)+
									      G[a1][d1] * (bval)*(cval);
		b[i + xoffset][j + yoffset] = B[b1][c1] * (aval)*(dval)+
										  B[a1][c1] * (bval)*(dval)+
										  B[b1][d1] * (aval)* (cval)+
										  B[a1][d1] * (bval)*(cval);	
		if (r[i][j] > 255)
			{
				r[i][j] = 255;
			}
			if (r[i][j] < 0)
			{
				r[i][j] = 0;
			}
			//r[i][j] = ppoint;
		
			if (g[i][j] > 255)
			{
				g[i][j] = 255;
		
			}
			else if (g[i][j] < 0)
			{
				
				g[i][j] = 0;
			}
				//g[i][j] = ppoint1;
			if (b[i][j]  > 255)
			{
				b[i][j] = 255;
			
			}
			else if (b[i][j]  < 0)
			{
				b[i][j] = 0;
			}								 		
			u=0;
			v=0;*/
       } 
    } 

	// 儲存處理結果至新的圖檔中
	//save_bmp("lenna_new1.bmp", r, g, b); // for true color images
	save_bmp("lena_new.bmp", r, r, r); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	system("PAUSE"); /* so that the command window holds a while */
	return 0;
}

