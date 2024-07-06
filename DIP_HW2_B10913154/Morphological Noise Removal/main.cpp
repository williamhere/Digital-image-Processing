/*===========================================================================
  This demonstrative example is provided by the teaching assistant,
  Mr. Shih-Hung Liao (���@��), and modified by the instructor, Prof. Lan.

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
#include <stdio.h>
#include <math.h>
#include "bmp.h"
	

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY]; // MaxBMPSizeX and MaxBMPSizeY are defined in "bmp.h"
//int G[1024][1024];
//int B[1024][1024];
int r[MaxBMPSizeX][MaxBMPSizeY];
int r1[MaxBMPSizeX][MaxBMPSizeY];
int r2[MaxBMPSizeX][MaxBMPSizeY];
int r3[MaxBMPSizeX][MaxBMPSizeY];

//int g[1024][1024];
//int b[1024][1024];

int main(int argc, char *argv[])
{
	int width, height;
	int i, j,u,v;
	char o=0;

	// �}�Ҩ�Ū�����m(24bits)bmp �v������
	// RGB �T�� channel ���Ȧs�J R, G, B �T�Ӧۭq���}�C��
	// �}�C�j�p���o�j�� 1024x1024 (�ثe�]�w) 
	// �Y�ݭn��j���v��size�A�i�Nbmp.h���� MaxBMPSizeX �P MaxBMPSizeY ���s�]�w 
	//open_bmp("lena.bmp", R, G, B, width, height); for true color images
	open_bmp("noisy_rectangle.bmp", R, R, R, width, height); // for gray images
	// The starting pixel of a bmp image is at the bottom-left corner, which has 
	// coordinates x=0 and y=0.  The last pixel is at the top-right corner.

    /*----------------------------------------------------------------------------
	      �d�ҡG���ͼƦ�t���]�бN�D�{����b���B�^
    ----------------------------------------------------------------------------*/
for (i = 0; i < width; i++){ //Erosion
		for (j = 0; j < height; j++){
			o=0;
			if(R[i][j]==255){
				for (u = -10; u < 11; u++){
					for (v = -10; v < 11; v++){
						if(R[i+u][j+v] == 0){
							o=1;
						}	
					}
				}
				if(o==0)
					r[i][j]=255;
				else
					r[i][j]=0;	
			}			
		}
	}
for (i = 0; i < width; i++){//Dilation
		for (j = 0; j < height; j++){
		if(r[i][j]==255)
			for (u = -10; u < 11; u++){
					for (v = -10; v < 11; v++){
					r1[i+u][j+v]=255; 
				}}				
			}		
	}
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
		if(r1[i][j]==255)
			for (u = -10; u < 11; u++){
					for (v = -10; v < 11; v++){
					r2[i+u][j+v]=255; 
				}}				
			}		
	}
for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			o=0;
			if(r2[i][j]==255){
				for (u = -10; u < 11; u++){
					for (v = -10; v < 11; v++){
						if(r2[i+u][j+v] == 0){
							o=1;
						}	
					}
				}
				if(o==0)
					r3[i][j]=255;
				else
					r3[i][j]=0;	
			}			
		}
	}	
	
	// �x�s�B�z���G�ܷs�����ɤ�
	//save_bmp("lenna_new1.bmp", r, g, b); // for true color images
	save_bmp("noisy_rectangle_new.bmp", r3, r3, r3); // for gray images
	printf("Job Finished!\n");

	// ���� bmp �v������
	close_bmp();

	system("PAUSE"); /* so that the command window holds a while */
	return 0;
}

