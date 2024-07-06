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
#include "bmp.cpp"
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; // MaxBMPSizeX and MaxBMPSizeY are defined in "bmp.h"
int process_R[MaxBMPSizeX+2][MaxBMPSizeY+2]={};
//int G[1024][1024];
//int B[1024][1024];
int r[MaxBMPSizeX][MaxBMPSizeY];
//int g[1024][1024];
//int b[1024][1024];
int o=3;
int Mask[9];
int XMask[25];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j,t,v,M;
	int c,z,x,y;

	// 開啟並讀取全彩(24bits)bmp 影像圖檔
	// RGB 三個 channel 的值存入 R, G, B 三個自訂的陣列中
	// 陣列大小不得大於 1024x1024 (目前設定)
	// 若需要更大的影像size，可將bmp.h中的 MaxBMPSizeX 與 MaxBMPSizeY 重新設定
	//open_bmp("lena.bmp", R, G, B, width, height); for true color images
	open_bmp("lena_pepper_and_salt_noise10%.bmp", R, R, R, width, height); // for gray images
	// The starting pixel of a bmp image is at the bottom-left corner, which has
	// coordinates x=0 and y=0.  The last pixel is at the top-right corner.

    /*----------------------------------------------------------------------------
	      範例：產生數位負片（請將主程式放在此處）
    ----------------------------------------------------------------------------*/
    for (j = height-1; j > -1; j--){ // typical image scanning order
	  	  for (i = 0; i < width; i++){

	   		process_R[i+1][j+1]=R[i][j];
        	if(i==0)
        		process_R[i][j]=process_R[i+1][j];
        	if(j==0)
        		process_R[i][j]=process_R[i][j+1];
        	if(i==width-1)
        		process_R[i+2][j]=process_R[i+1][j];
        	if(j==height-1)
        		process_R[i][j+2]=process_R[i][j+1];
       } // r[0][0] is located at the bottom left corner.
    } // i: horizontal direction, j: vertical direction
    	process_R[0][0]=process_R[1][1];	
		process_R[0][1025]=process_R[1][1024];	
		process_R[1025][0]=process_R[1024][1];	
		process_R[1025][1025]=process_R[1024][1024];
    
    
    
	for (j = height; j > 0; j--){ // typical image scanning order
	   for (i = 1; i < width+1; i++){
	   	c=0;
			for(t=-1;t<2;t++){
				for(v=-1;v<2;v++){
					Mask[c] = process_R[i+t][j+v];
					c+=1;
					//printf("%d\n",Mask[c]);
				}	
			}
			for(c=8;c>0;c--){
				M=Mask[0];
				z=0;
				for(x=1;x<=c;x++){
					if(M<=Mask[x]){
						
						M=Mask[x];
						z=x;
					}
				}	
				y=Mask[c];
				Mask[c]=M;
				Mask[z]=y;
			}
//			for(c=0;c<25;c++)
//				printf("%d ",Mask[c]);
//			printf("\n",Mask[c]);
			if(process_R[i][j]<Mask[8] && process_R[i][j]>Mask[0] )
				r[i-1][j-1]=process_R[i][j];
			else
				if(Mask[4]<Mask[8] && Mask[4]>Mask[0] )
					r[i-1][j-1]=Mask[4];
				else{
						for(t=-2;t<3;t++){
							for(v=-2;v<3;v++){
								XMask[c] = process_R[i+t][j+v];
								c+=1;
								//printf("%d\n",Mask[c]);
							}	
						}
					for(c=24;c>0;c--){
						M=XMask[0];
						z=0;
						for(x=1;x<=c;x++){
							if(M<=XMask[x]){
								
								M=XMask[x];
								z=x;
							}
						}	
						y=XMask[c];
						XMask[c]=M;
						XMask[z]=y;
					}
					if(process_R[i][j]<XMask[24] && process_R[i][j]>XMask[0] )
						r[i-1][j-1]=process_R[i][j];
					else
						if(XMask[12]<XMask[24] && XMask[12]>XMask[0] )
							r[i-1][j-1]=XMask[4];
						}
				
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
