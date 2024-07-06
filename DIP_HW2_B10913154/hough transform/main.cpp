#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include <string>

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
float R1[MaxBMPSizeX][MaxBMPSizeY];
float R2[MaxBMPSizeX][MaxBMPSizeY];
float R21[MaxBMPSizeX][MaxBMPSizeY];
float R22[MaxBMPSizeX][MaxBMPSizeY];

int r1[MaxBMPSizeX][MaxBMPSizeY];
int r2[MaxBMPSizeX][MaxBMPSizeY];
int r21[MaxBMPSizeX][MaxBMPSizeY];
int r22[MaxBMPSizeX][MaxBMPSizeY];
int r3[MaxBMPSizeX][MaxBMPSizeY];
int r31[MaxBMPSizeX][MaxBMPSizeY];
int r32[MaxBMPSizeX][MaxBMPSizeY];
int r4[MaxBMPSizeX][MaxBMPSizeY];

float strength[MaxBMPSizeX][MaxBMPSizeY];
float strength1[MaxBMPSizeX][MaxBMPSizeY];
float theta[MaxBMPSizeX][MaxBMPSizeY];
int theta1[MaxBMPSizeX][MaxBMPSizeY];
int weak[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[]){
	
int WIDTH, HEIGHT;
float t1[25] ,t2=0 ,t3[25]; 
float sigma=0.000001; 
float pi=3.141592;
float val=180/pi;
int i ,j ,k ,m ,m1 ,m2 ,E=0;

open_bmp("coins.bmp",R ,R ,R ,WIDTH ,HEIGHT);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////高斯低通濾波(5x5)
for (j = -2; j < 3; j++){ 
	for (i = -2; i < 3; i++){
 	     t1[0+E] = exp(-(i*i + j*j)/(2*sigma*sigma));
 	     t2 = t2 + t1[0+E];
 	     E++;
    } 
}
for (i = 0; i < 25; i++) 	        
     t3[i]=(1/t2)*t1[i];
    
for (j = 0; j < HEIGHT; j++){ 
	for (i = 0; i < WIDTH; i++){ 	
        R1[i][j] = R[i - 2][j + 2]*t3[0]  + R[i - 1][j + 2]*t3[1]  + R[i + 0][j + 2]*t3[2]  + R[i+1][j + 2]*t3[3]  + R[i + 2][j + 2]*t3[4]   
			     + R[i - 2][j + 1]*t3[5]  + R[i - 1][j + 1]*t3[6]  + R[i + 0][j + 1]*t3[7]  + R[i+1][j + 1]*t3[8]  + R[i + 2][j + 1]*t3[9]   
			     + R[i - 2][j + 0]*t3[10] + R[i - 1][j + 0]*t3[11] + R[i + 0][j + 0]*t3[12] + R[i+1][j + 0]*t3[13] + R[i + 2][j + 0]*t3[14]   
			     + R[i - 2][j - 1]*t3[15] + R[i - 1][j - 1]*t3[16] + R[i + 0][j - 1]*t3[17] + R[i+1][j - 1]*t3[18] + R[i + 2][j - 1]*t3[19]  
				 + R[i - 2][j - 2]*t3[20] + R[i - 1][j - 2]*t3[21] + R[i + 0][j - 2]*t3[22] + R[i+1][j - 2]*t3[23] + R[i + 2][j - 2]*t3[24] ;
    } 
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for (j = 0; j < HEIGHT; j++) {
	for (i = 0; i < WIDTH; i++){
		R21[i][j] = R1[i - 1][j + 1]*-1 + R1[i + 0][j + 1]* 0 + R1[i + 1][j + 1]* 1    ///////////////X
			      + R1[i - 1][j + 0]*-2 + R1[i + 0][j + 0]* 0 + R1[i + 1][j + 0]* 2
			      + R1[i - 1][j - 1]*-1 + R1[i + 0][j - 1]* 0 + R1[i + 1][j - 1]* 1;
		R22[i][j] = R1[i - 1][j + 1]*-1 + R1[i + 0][j + 1]*-2 + R1[i + 1][j + 1]*-1    //////////////Y
			      + R1[i - 1][j + 0]* 0 + R1[i + 0][j + 0]* 0 + R1[i + 1][j + 0]* 0
			      + R1[i - 1][j - 1]* 1 + R1[i + 0][j - 1]* 2 + R1[i + 1][j - 1]* 1;
		                         
	    if((fabs(R21[i][j]) + fabs(R22[i][j])) > 255)	                             /////////////|X|+|Y|
	 	     R2[i][j] = 255;
    	else
  	         R2[i][j] = fabs(R21[i][j]) + fabs(R22[i][j]);
	
	     strength[i][j] = sqrt(R22[i][j]*R22[i][j] + R21[i][j]*R21[i][j]);         /////////////計算梯度大小		       
      	 theta[i][j] = (atan2(R22[i][j],R21[i][j])*val);                           /////////////計算梯度角度  
		   	
    	if (theta[i][j] < 0)                                                    
	         theta[i][j] = theta[i][j] + 360;
	         
	    if (theta[i][j] >= 0){                                                     ///////////量化角度成8等分 
        	if ((22.5 >= theta[i][j] && theta[i][j] >= 0) || (360.0 >= theta[i][j] && theta[i][j] > 337.5))
        	     theta1[i][j] = 0;	
	        if (67.5 >= theta[i][j] && theta[i][j] > 22.5)
         	     theta1[i][j] = 45;
    	    if (112.5 >= theta[i][j] && theta[i][j] > 67.5)
         	     theta1[i][j] = 90;
    	    if (157.5 >= theta[i][j] && theta[i][j] > 112.5)
	   	         theta1[i][j] = 135;
    	    if (202.5 >= theta[i][j] && theta[i][j] > 157.5)
         	     theta1[i][j] = 180;
        	if (247.5 >= theta[i][j] && theta[i][j] > 202.5)
         	     theta1[i][j] = 225;
        	if (292.5 >= theta[i][j] && theta[i][j] > 247.5)
     	         theta1[i][j] = 270;
        	if (337.5 >= theta[i][j] && theta[i][j] > 292.5)
     	         theta1[i][j] = 315;		
         }	                           		                	        	                
	 }
}       
/////////////////////////////////////////////////////////////////////////////////////////////////////////////非極大值抑制	
for (j = 0; j < HEIGHT; j++) {                                                    
    for (i = 0; i < WIDTH; i++){
        if(theta1[i][j] == 0 || theta1[i][j] == 180){
            if(strength[i][j] >= strength[i+1][j] || strength[i][j] >= strength[i-1][j])
               	strength1[i][j] = strength[i][j];				
			if(strength[i][j] < max(strength[i][j] ,max(strength[i+1][j] ,strength[i-1][j]))) 
		 	    strength1[i][j] = 0;   
		} 	  	
        if(theta1[i][j] == 45 || theta1[i][j] == 225){
         	if(strength[i][j] >= strength[i+1][j+1] || strength[i][j] >= strength[i-1][j-1])
               	strength1[i][j] = strength[i][j];				
			if(strength[i][j] < max(strength[i][j] ,max(strength[i+1][j+1] ,strength[i-1][j-1]))) 
		 	    strength1[i][j] = 0;    
		}
        if(theta1[i][j] == 90 || theta1[i][j] == 270){
            if(strength[i][j] >= strength[i][j+1] || strength[i][j] >= strength[i][j-1])
               	strength1[i][j] = strength[i][j];				
			if(strength[i][j] < max(strength[i][j] ,max(strength[i][j+1] ,strength[i][j-1]))) 
		 	    strength1[i][j] = 0;   
		}
        if(theta1[i][j] == 135 || theta1[i][j] == 315){
            if(strength[i][j] >= strength[i-1][j+1] || strength[i][j] >= strength[i+1][j-1])
               	strength1[i][j] = strength[i][j];				
			if(strength[i][j] < max(strength[i][j] ,max(strength[i-1][j+1] ,strength[i+1][j-1])))
		 	    strength1[i][j] = 0; 
		}  		   			   
	 }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////weak_edge_tracking
for (j = 0; j < HEIGHT; j++) {
	for (i = 0; i < WIDTH; i++){
        if(strength1[i][j] >= 420){/////////////////////////////////////////////////////th
            r1[i][j] = 255;
			weak[i][j] = 0;				  		  	
		 }
		if(strength1[i][j] <= 105){/////////////////////////////////////////////////////tl         	   
            r1[i][j] = 0;
            weak[i][j] = 0;			                   
		 }
		if(strength1[i][j] < 420 && strength1[i][j] > 105){ ///////////////////////////weak			
		    weak[i][j] = 1;
	   	 }		   
     } 	
}
//////////////////////////////////////////////////////////////////////////////////////Edge tracking by hysteresis(三種情況)
for(k = 0; k < 10 ; k++){		
    for (j = 0; j < HEIGHT; j++) {
	    for (i = 0; i < WIDTH; i++){						  	  	
            if(weak[i][j] == 1){                      	
			    if(r1[i-1][j+1] == 255 || r1[i][j+1] == 255 || r1[i+1][j+1] == 255 || 
			       r1[i-1][j] == 255   || r1[i+1][j] == 255 || r1[i-1][j-1] == 255 ||
			       r1[i][j-1] == 255   || r1[i-1][j-1] == 255){
			   	         r1[i][j] = 255;			          	
			             weak[i][j] = 0; 
				 }				
		        if(r1[i-1][j+1] == 0 && r1[i][j+1] == 0 && r1[i+1][j+1] == 0 && 
			       r1[i-1][j] == 0 && r1[i+1][j] == 0 && r1[i-1][j-1] == 0 &&
			       r1[i][j-1] == 0 && r1[i-1][j-1] == 0){
	                     r1[i][j] = 0;	
			             weak[i][j] = 0;											   		           	
				 }							     	
		     }			
         }
     }
}
/////////////////////////////////////////////////////////////////////////////////////////////
for (j = 0; j < HEIGHT; j++){ 
	for (i = 0; i < WIDTH; i++){
    	if(r1[i][j] == 255){
            for(m = 0; m < 360 ; m++){
			    for(k = 25; k < 27; k++){        	 	
          	         m1 = k*cos(m/val); 
          	         m2 = k*sin(m/val); 
                     r21[i + m1][j + m2] = r21[i + m1][j + m2] + 1;
                 }
                for(k = 29; k < 31; k++){        	 	
          	         m1 = k*cos(m/val);  
          	         m2 = k*sin(m/val);  
                     r22[i + m1][j + m2] = r22[i + m1][j + m2] + 1;
                 } 
	         }
	     }
     }
}
///////////////////////////////////////////////////////////////////////////////////////////////
for (j = 0; j < HEIGHT; j++){ 
	for (i = 0; i < WIDTH; i++){
    	if(r21[i][j] < 149)
		     r21[i][j] = 0;
		else
	         r21[i][j] = 255;			 	 
		if(r22[i][j] < 161)
		     r22[i][j] = 0;
		else
	         r22[i][j] = 255;
				  
        if(r21[i][j] == 255){		       
            for(m = 0; m < 360 ; m++){
			    for(k = 25; k < 27; k++){        	 	
          	         m1 = k*cos(m/val); 
          	 	     m2 = k*sin(m/val);    
				     r31[i + m1][j + m2] = 255;         	           	  
		     	 } 
	       	 }
 		 }
	    if(r22[i][j] == 255){		       
            for(m = 0; m < 360 ; m++){ 
		        for(k = 29; k < 31; k++){       	 	
          	         m1 = k*cos(m/val);
          	 	     m2 = k*sin(m/val);  
				     r32[i + m1][j + m2] = 255;  	
			     } 
	       	 }
 		 }		  		   	
 	 }		 
} 	
/////////////////////////////////////////////////////////////////////////////////////////////
for (j = 0; j < HEIGHT; j++){ 
	for (i = 0; i < WIDTH; i++){
	     r2[i][j] = r21[i][j] + r22[i][j];
	     r3[i][j] = r31[i][j] + r32[i][j];	    		    	    	     
         r4[i][j] = r3[i][j] + R[i][j];
    if(r4[i][j] > 255)
         r4[i][j] = 255;      
	 }
}		
/////////////////////////////////////////////////////////////////////////////////////////////
save_bmp("coins_new1.bmp", r1, r1, r1);
save_bmp("coins_new2.bmp", r2, r2, r2);
save_bmp("coins_new3.bmp", r3, r3, r3);
save_bmp("coins_new4.bmp", r4, r4, r4);
		    
printf("Job Finished!\n");
	   			   
close_bmp();
system("PAUSE"); 
return 0;
}

