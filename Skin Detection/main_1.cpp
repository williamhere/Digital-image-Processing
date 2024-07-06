#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include <algorithm>
using namespace std;
int R[MaxBMPSizeX][MaxBMPSizeY]; 
int G[MaxBMPSizeX][MaxBMPSizeY]; 
int B[MaxBMPSizeX][MaxBMPSizeY]; 
int r[MaxBMPSizeX][MaxBMPSizeY]; 
int g[MaxBMPSizeX][MaxBMPSizeY]; 
int b[MaxBMPSizeX][MaxBMPSizeY]; 
float H[MaxBMPSizeX][MaxBMPSizeY];
float S[MaxBMPSizeX][MaxBMPSizeY];
float V[MaxBMPSizeX][MaxBMPSizeY];
float delta[MaxBMPSizeX][MaxBMPSizeY];
float h[MaxBMPSizeX][MaxBMPSizeY];
int HH[MaxBMPSizeX][MaxBMPSizeY];
float F[MaxBMPSizeX][MaxBMPSizeY];
float P[MaxBMPSizeX][MaxBMPSizeY];
float Q[MaxBMPSizeX][MaxBMPSizeY];
float T[MaxBMPSizeX][MaxBMPSizeY];
float d[MaxBMPSizeX][MaxBMPSizeY];
int main(int argc,char *argv[])
{
	int  width, height;
	int i, j;
	open_bmp("skin_det_1.bmp", R, G, B, width, height);
	for (j = height-1; j > -1; j--){ 
	   for (i = 0; i < width; i++){ 
         V[i][j]=max(R[i][j], max(G[i][j], B[i][j]));
         d[i][j]=V[i][j]-min(R[i][j], min(G[i][j], B[i][j]));
         delta[i][j]=d[i][j];
         S[i][j]=delta[i][j]/(V[i][j]);
         
		 if(V[i][j]==R[i][j])
        H[i][j]=(1/6.0)*((G[i][j]-B[i][j])/delta[i][j]);
        
		 if(V[i][j]==G[i][j])
         H[i][j]=(1/6.0)*(2+((B[i][j]-R[i][j])/delta[i][j]));
         
		 if(V[i][j]==B[i][j])
         H[i][j]=(1/6.0)*(4+((R[i][j]-G[i][j])/delta[i][j]));
         
         if(R[i][j]==B[i][j] && R[i][j]==G[i][j])
				H[i][j]=0;
				
         if(H[i][j]<0)
         H[i][j]=H[i][j]+1;
         
      h[i][j]=H[i][j] *360.0;
           /*  if((5.0) >= h[i][j] || h[i][j] >= (43.4) || 0.09 >= S[i][j] || S[i][j] >= 1.0|| 140 >= V[i][j] ){
         H[i][j] = 0;
		 S[i][j] = 0;	   
		 V[i][j] = 0;	   	   
		}*/	 
        if((0.0<=h[i][j])&&(h[i][j]<=41.0)&&(0.01<=S[i][j])&&(S[i][j]<=1.0)){
        V[i][j]=V[i][j];
        S[i][j]=S[i][j];
        H[i][j]=H[i][j];
		 }
		 else{
	    V[i][j]=0;
        S[i][j]=0;
        H[i][j]=0;
		 	 
		 }//*/
       } 
       
    } 
  	for (j = height-1; j > -1; j--){ 
	   for (i = 0; i < width; i++){ 
	   HH[i][j]=(6*H[i][j]);
	   F[i][j]=6.0*H[i][j]-HH[i][j];
       P[i][j]=V[i][j]*(1.0-S[i][j]);
       Q[i][j]=V[i][j]*(1.0-S[i][j]*F[i][j]);
       T[i][j]=V[i][j]*(1.0-S[i][j]*(1.0-F[i][j]));
       
       if(HH[i][j]==0){
       	r[i][j]=V[i][j];
     	g[i][j]=T[i][j];
       	b[i][j]=P[i][j];
	   }
	   else if(HH[i][j]==1){
       	r[i][j]=Q[i][j];
       	g[i][j]=V[i][j];
       	b[i][j]=P[i][j];
	   }
	   else if(HH[i][j]==2){
       	r[i][j]=P[i][j];
       	g[i][j]=V[i][j];
       	b[i][j]=T[i][j];
	   }
	   else if(HH[i][j]==3){
       	r[i][j]=P[i][j];
       	g[i][j]=Q[i][j];
       	b[i][j]=V[i][j];
	   }
	   else if(HH[i][j]==4){
       	r[i][j]=T[i][j];
       	g[i][j]=P[i][j];
       	b[i][j]=V[i][j];
	   }
	   else if(HH[i][j]==5){
       	r[i][j]=V[i][j];
       	g[i][j]=P[i][j];
       	b[i][j]=Q[i][j];
	   }
     if(r[i][j]==g[i][j]&&r[i][j]==b[i][j]){
     	r[i][j]=0;
		g[i][j]=0;
		b[i][j]=0; 
	 }
      
}

    
}

	save_bmp("skin_det_1_new.bmp", r, g, b); 
	printf("Job Finished!\n");
	close_bmp();
	system("PAUSE");
	return 0;
}
