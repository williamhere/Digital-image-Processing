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
//int G[1024][1024];
//int B[1024][1024];
int r[MaxBMPSizeX][MaxBMPSizeY];
//int g[1024][1024];
//int b[1024][1024];
unsigned int  windowsize;  // 濾波器現在視窗大小 
unsigned int  c[9];       // 濾波器視窗的起始尺寸
unsigned int  d[121];     // 濾波器變大視窗大小 

int main(int argc, char *argv[])
{
	int width,height;
	int i,j,x,y,t,z;
	int Zmax,Zmin,Zxy,Zmed;
	int A1=Zmed-Zmin;
	int A2=Zned-Zmax;
	int B1=Zxy-Zmin;
	int B2=Zxy-Zmax;
	

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
       
    
       for (j = 1; j < height-1; j++){ // typical image scanning order
	      for (i = 1; i < width-1; i++){
	        t=0;                     //c0~c9 
	          for(y=1;y<=-2;y--){   
	          	for(x=-1;x<2;x++){
	          	  c[t]=R[i+x][j+y];
	          	  t=t+1;
				  }
	          	
	          } 
	               Zmed=zmed_value(c);
	               Zmin=zmin_value(c);
	               Zmax=zmax_value(c);       
	      if(A1>0 && A2<0)              //start to judge the stage A and stage B
		  {
	      	  if(B1>0 && B2<0)
				{
	      	  	    r[i][j]=Zxy;
	      	    }
	      	  else
				{
	      	  	    r[i][j]=Zmed;
	      	    }
	      }
	      else
		  {  
			z=0;                  //d0~d121
	        for(y=-5;y<5;y++){
	          for(x=-5;x<5;x++){
	          	  d[z]=r[y][x];
	          	  z=z+1;
				  }	
	          }
	      Zmed=zmed_value1(d);
	      Zmin=zmin_value1(d);
	      Zmax=zmax_value1(d);
		  if(A1>0 && A2<0)              //start to judge the new stage A and stage B
		  {
	      	  if(B1>0 && B2<0)
				{
	      	  	    r[i][j]=Zxy;
	      	    }
	      	  else
				{
	      	  	    r[i][j]=Zmed;
	      	    }
	      } 
		  else
		  {
		  	r[i][j]=Zmed;
		  } 
	    }
	      
	   }
  }
  
  int zmed_value(unsigned int c[9])
  {
  	int i,j,buf;
  	for(i=0;i<9;i++)
	{
  	if(c[i+1]<c[i])//升冪 
	  	{
  		buf=c[i+1];
  		c[i+1]=c[i];
  		c[i]=buf;
  		}
  	}
  	
	  return c[4]; 
  }
  int zmax_value(unsigned int c[9])
  {
  	int i,j,max;
  		for(i=0;i<9;i++){
  		     if(c[i+1]>c[i]){
  		     	max=c[i+1];
  		     }
  		}
  	
  	return c[i+1];
  }
  int zmin_value(unsigned int c[9])
  {
  	int i,j,min;
  	for(j=0;j<2;j++){
  		for(i=0;i<2;i++){
  		     if(c[i+1]>c[i]){
  		     	min=c[i];
  		     }
  		}
  	}
  	return c[i];
  }
  int zmed_value1(unsigned int d[121])
  {
  	int i,j,buf;
  	for(j=0;j<10;j++){
  		for(i=0;i<10;i++){
  			if(d[i+1]<d[i]){
  				buf=d[i+1];
  				d[i+1]=d[i];
  				d[i]=buf;
  			}
  		}
  	}
	  return d[4]; 
  }
  int zmax_value1(unsigned int d[121])
  {
  	int i,j,max;
  	for(j=0;j<10;j++){
  		for(i=0;i<10;i++){
  		     if(d[i+1]>d[i]){
  		     	max=d[i+1];
  		     }
  		}
  	}
  	return d[i+1];
  }
  int zmin_value1(unsigned int d[121])
  {
  	int i,j,min;
  	for(j=0;j<10;j++){
  		for(i=0;i<10;i++){
  		     if(d[i+1]>d[i]){
  		     	min=d[i];
  		     }
  		}
  	}
  	return d[i];
  }

	// 儲存處理結果至新的圖檔中
	//save_bmp("lenna_new1.bmp", r, g, b); // for true color images
	save_bmp("lena_new4.bmp", r, r, r); // for gray images
	printf("Job Finished!\n");

	// 關閉 bmp 影像圖檔
	close_bmp();

	system("PAUSE"); /* so that the command window holds a while */
	return 0;
}
