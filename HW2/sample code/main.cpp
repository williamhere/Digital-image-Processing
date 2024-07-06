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
//int G[1024][1024];
//int B[1024][1024];
int r[MaxBMPSizeX][MaxBMPSizeY];
//int g[1024][1024];
//int b[1024][1024];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j;

	// 開啟並讀取全彩(24bits)bmp 影像圖檔
	// RGB 三個 channel 的值存入 R, G, B 三個自訂的陣列中
	// 陣列大小不得大於 1024x1024 (目前設定) 
	// 若需要更大的影像size，可將bmp.h中的 MaxBMPSizeX 與 MaxBMPSizeY 重新設定 
	//open_bmp("lena.bmp", R, G, B, width, height); for true color images
	open_bmp("lena.bmp", R, R, R, width, height); // for gray images
	// The starting pixel of a bmp image is at the bottom-left corner, which has 
	// coordinates x=0 and y=0.  The last pixel is at the top-right corner.

    /*----------------------------------------------------------------------------
	      範例：產生數位負片（請將主程式放在此處）
    ----------------------------------------------------------------------------*/
	for (j = height-1; j > -1; j--){ // typical image scanning order
	   for (i = 0; i < width; i++){ 
	/* for (i=0; i < width; i++){
	   for (j=0; j < height; j++){ */ 
         r[i][j]=255-R[i][j];
         //g[i][j] = 255-G[i][j];
         //b[i][j] = 255-B[i][j];
       } // r[0][0] is located at the bottom left corner.
    } // i: horizontal direction, j: vertical direction

	// 儲存處理結果至新的圖檔中
	//save_bmp("lenna_new1.bmp", r, g, b); // for true color images
	save_bmp("lena_new.bmp", r, r, r); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	system("PAUSE"); /* so that the command window holds a while */
	return 0;
}
