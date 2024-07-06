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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "bmp.cpp"
#define sharp_size 15

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; // MaxBMPSizeX and MaxBMPSizeY are defined in "bmp.h"

int rrr[MaxBMPSizeX][MaxBMPSizeY];

int rr[MaxBMPSizeX][MaxBMPSizeY];

int r[MaxBMPSizeX][MaxBMPSizeY];

int img[MaxBMPSizeX][MaxBMPSizeY];




int main(int argc, char *argv[])
{
	
	int width, height;
	int i, j,u,v;
	char ox=0;
	

	open_bmp("noisy_rectangle.bmp", R, R, R, width, height); // for gray images

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			ox=0;
			if(R[i][j]==255){
				for (u = -((int)sharp_size); u < ((int)sharp_size+1); u++){
					for (v = -((int)sharp_size); v < ((int)sharp_size+1); v++){
						if(R[i+u][j+v] == 0){
							ox=1;
						}	
					}
				}
				if(ox==0)
					rrr[i][j]=255;
				else
					rrr[i][j]=0;	
			}
			
		}
	}
	
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			if(rrr[i][j]==255){
				for (u = -((int)sharp_size); u < ((int)sharp_size+1); u++){
					for (v = -((int)sharp_size); v < ((int)sharp_size+1); v++){
						rr[i+u][j+v]=255;
					}
				}
			}	
		}
	}
	
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			if(rr[i][j]==255){
				for (u = -((int)sharp_size); u < ((int)sharp_size+1); u++){
					for (v = -((int)sharp_size); v < ((int)sharp_size+1); v++){
						r[i+u][j+v]=255;
					}
				}
			}	
		}
	}

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			ox=0;
			if(r[i][j]==255){
				for (u = -((int)sharp_size); u < ((int)sharp_size+1); u++){
					for (v = -((int)sharp_size); v < ((int)sharp_size+1); v++){
						if(r[i+u][j+v] == 0){
							ox=1;
						}	
					}
				}
				if(ox==0)
					img[i][j]=255;
				else
					img[i][j]=0;	
			}
			
		}
	}
	

	// 儲存處理結果至新的圖檔中
	//save_bmp("cameraman_new.bmp", r, g, b); // for true color images
	
	save_bmp("noisy_rectangle_1.bmp", rrr, rrr, rrr);
	
	save_bmp("noisy_rectangle_2.bmp", rr, rr, rr);
	
	save_bmp("noisy_rectangle_3.bmp", r, r, r );
	
	save_bmp("noisy_rectangle_find.bmp", img, img, img ); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	system("PAUSE"); /* so that the command window holds a while */
	return 0;
}
