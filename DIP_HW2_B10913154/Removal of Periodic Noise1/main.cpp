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
#include "FFT1.C"
#include "FFT2.C"
//#include "bmp.cpp" 

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; // MaxBMPSizeX and MaxBMPSizeY are defined in "bmp.h"
int r[MaxBMPSizeX][MaxBMPSizeY];
float A[2 * MaxBMPSizeX * MaxBMPSizeY];
double fft[MaxBMPSizeX][MaxBMPSizeY][2] = { 0 };
float H[MaxBMPSizeX][MaxBMPSizeY] = { 0 };
double Butterworth[MaxBMPSizeX][MaxBMPSizeY][2] = { 0 };

int D_0 = 1;
int n = 1;


int main(int argc, char *argv[])
{
	
	float sigma = 0;
	int width, height;
	int i, j,u,v;
	int a,b;

	open_bmp("periodic_noise2.bmp", R, R, R, width, height); // for gray images

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			A[(j+i*height)*2] = R[i][j];
			A[(j+i*height)*2+1] = 0;
		}
	}

	
	fft2(A,width,1);

	
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			if(i>=width/2 && j>=height/2){
				fft[i - width/2][j - height/2][0] = A[(j+i*height)*2];
				fft[i - width/2][j - height/2][1] = A[(j+i*height)*2+1];
			}
			if(i<width/2 && j>=height/2){
				fft[i + width/2][j - height/2][0] = A[(j+i*height)*2];
				fft[i + width/2][j - height/2][1] = A[(j+i*height)*2+1];
			}
			if(i<width/2 && j<height/2){
				fft[i + width/2][j + height/2][0] = A[(j+i*height)*2];
				fft[i + width/2][j + height/2][1] = A[(j+i*height)*2+1];
			}
			if(i>=width/2 && j < height/2){
				fft[i - width/2][j + height/2][0] = A[(j+i*height)*2];
				fft[i - width/2][j + height/2][1] = A[(j+i*height)*2+1];
			}
		}
	}

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			r[i][j] =int(log(1 + sqrt(pow(fft[i][j][0],2) + pow(fft[i][j][1],2)))*10  );
			H[i][j] = 1;
		} 
	}	
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){

			if(r[i][j]==146){
				H[i][j] = 1;
				for (u = 0; u < width; u++){
					for (v = 0; v < height; v++){
						if(H[u][v] == 1)
							H[u][v] = 1- (1/(1 + pow(sqrt(pow(i-u,2)+pow(j-v,2))/D_0,2*n)));
						
						Butterworth[u][v][0] = fft[u][v][0] * H[u][v];											//將實部的值與濾波器係數相乘並存入實部
						Butterworth[u][v][1] = fft[u][v][1] * H[u][v];											//將虛部的值與濾波器係數相乘並存入虛部
						
					}
				}
			}	
		} 
	}	
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			if(H[i][j]==0)
			//printf("%f  ",H[i][j]);
			if(i>=width/2 && j>=height/2){
				A[(j+i*height)*2] = Butterworth[i - width/2][j - height/2][0] ;
				A[(j+i*height)+1] = Butterworth[i - width/2][j - height/2][1];
			}
			if(i<width/2 && j>=height/2){
				A[(j+i*height)*2] = Butterworth[i + width/2][j - height/2][0];
				A[(j+i*height)*2+1] = Butterworth[i + width/2][j - height/2][1];
			}
			if(i<width/2 && j<height/2){
				A[(j+i*height)*2] = Butterworth[i + width/2][j + height/2][0];
				A[(j+i*height)*2+1] = Butterworth[i + width/2][j + height/2][1];
			}
			if(i>=width/2 && j < height/2){
				A[(j+i*height)*2] = Butterworth[i - width/2][j + height/2][0];
				A[(j+i*height)*2+1] = Butterworth[i - width/2][j + height/2][1];
			}
		}
	}
	fft2(A,width,-1);
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			r[i][j] = (int)(sqrt(pow(A[(j+i*height)*2],2)+pow(A[(j+i*height)*2+1] ,2)));
			//A[(j+i*height)*2+1] = 0;
		}
	}



	save_bmp("periodic_noise2_new2.bmp", r, r, r ); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	system("PAUSE"); /* so that the command window holds a while */
	return 0;
}

