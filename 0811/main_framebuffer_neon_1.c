#include "inc_camera.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arm_neon.h>
//------------------
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<linux/fb.h>
#include<sys/mman.h>
#include <time.h>



#define PI 3.1415

int width = 1280;
int height = 720;

int main(int argc, char *argv[])
{
    float val = 180 / PI;
	int dev=0;
	unsigned int pixelformat = V4L2_PIX_FMT_YUYV;
	unsigned int nbufs = V4L_BUFFERS_DEFAULT;
	unsigned int input = 0;
	unsigned int i , j;
	int ret;
	int frame=1;
	int s=0;
	unsigned char R,G,B,Y_OUT,U_OUT,V_OUT;
	unsigned char *Y     = calloc(width * height,sizeof(unsigned char));
	unsigned char *Y1    = calloc(width * height,sizeof(unsigned char));
	unsigned char *Y2    = calloc(width * height,sizeof(unsigned char));
	unsigned char *Y3    = calloc(width * height,sizeof(unsigned char));
	unsigned char *Y4    = calloc(width * height,sizeof(unsigned char));
	int *YD    = calloc(width * height,sizeof(int));
	int *YY    = calloc(width * height,sizeof(int));
    	int *YX    = calloc(width * height,sizeof(int));
	unsigned char *Y_out = calloc(width * height,sizeof(unsigned char));

	unsigned char *Y_1_1 = calloc(width * height,sizeof(unsigned char)); //Left down
	unsigned char *Y_10  = calloc(width * height,sizeof(unsigned char));  //Left
	unsigned char *Y_11  = calloc(width * height,sizeof(unsigned char));  //Left top
	unsigned char *Y0_1  = calloc(width * height,sizeof(unsigned char));  //down
	unsigned char *Y01   = calloc(width * height,sizeof(unsigned char));   //top
	unsigned char *Y1_1  = calloc(width * height,sizeof(unsigned char));  //Right down
	unsigned char *Y10   = calloc(width * height,sizeof(unsigned char));   //Right
	unsigned char *Y11   = calloc(width * height,sizeof(unsigned char));   //Right top
	double all_time=0;
	double av_time=0;

	clock_t clockbegin,clockend;


	BT656_buffer =(unsigned char*)malloc(sizeof(unsigned char) * lsize*wsize*2);

	dev = video_open("/dev/video0");
	camera_init(input,pixelformat,nbufs,0,dev,width,height);
	memset(&buf, 0, sizeof buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	int YAll = 0;
	//-------------------------------------------------------------------------------------
	int fp = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long screensize = 0;
	char* fbp = 0;
	long location = 0;
	fp = open("/dev/fb0",O_RDWR);
	if(fp<0)
	{
		printf("Open device failed.\n");
		exit(1);
	}
	if(ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
	{
		printf("ERROR reading fixed info\n");
		exit(2);
	}
	printf("mem = %d\n",finfo.smem_len);
	printf("line_length = %d\n",finfo.line_length);
	if(ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
	{
		printf("ERROR reading variable info\n");
		exit(3);
	}
	printf("xres = %d\n",vinfo.xres);
	printf("yres = %d\n",vinfo.yres);
	printf("bits_per_pixel = %d\n",vinfo.bits_per_pixel);
	screensize = vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;
	fbp = (char*)mmap(0,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0);
	if((int)fbp == -1)
	{
		printf("ERROR to map\n");
		exit(4);
	}
	//-------------------------------------------------------------------------------------

    double a = 0.3;
//    double exp2 =exp(-2.0/(2.0*a));
//    double exp1 =exp(-1.0/(2.0*a));
    double exp2 =0.0356739;
    double exp1 =0.1888756;
    int avg = 0; 
//pow(2.0,2.0)
	while(1)
	{

		ioctl(dev,VIDIOC_DQBUF,&buf);
		clockbegin=clock();
		memcpy(BT656_buffer , mem[buf.index], width*height*2);
        /*YUYV get Y*/
		for (int ix = 0; ix < height; ix++){
			for (int jx = 0; jx < width; jx++){
				Y[ix*width + jx] = BT656_buffer[ix*width * 2 + jx * 2];

			}
		}
        /*Gaussian Blur*/
       for(int ix = 1;ix < height-1;ix++){
            for(int jx = 1;jx < width-1;jx++){
                int con=ix*width+jx;
		avg = avg + Y[con];
                Y1[con] = (Y[con - width - 1] * exp2  + Y[con - width] * exp1 + Y[con - width + 1] * exp2
                        + Y[con - 1] * exp1 	      + Y[con] * (1) 	      + Y[con + 1] * exp1
                        + Y[con + width - 1] * exp2  + Y[con + width ] * exp1+ Y[con + width + 1] * exp2)/(exp1*4+exp2*4+1);
               }
        }
	avg = avg/(width*height);

        /*Canny */
        for(int ix = 1;ix < height-1;ix++){
            for(int jx = 1;jx < width-1;jx++){
                int con=ix*width+jx;

                YY[con] = Y1[(ix - 1)*width+(jx - 1)] * (+1) + Y1[(ix - 1)*width+(jx - 0)] * (+2) + Y1[(ix - 1)*width+(jx + 1)] * (+1)
                        + Y1[(ix + 1)*width+(jx - 1)] * (-1) + Y1[(ix + 1)*width+(jx - 0)] * (-2) + Y1[(ix + 1)*width+(jx + 1)] * (-1);
                YX[con] = Y1[(ix - 1)*width+(jx - 1)] * (+1) + Y1[(ix + 0)*width+(jx - 1)] * (+2) + Y1[(ix + 1)*width+(jx - 1)] * (+1)
                        + Y1[(ix - 1)*width+(jx + 1)] * (-1) + Y1[(ix + 0)*width+(jx + 1)] * (-2) + Y1[(ix + 1)*width+(jx + 1)] * (-1);
                if((abs(YX[con]) + abs(YY[con])) > 255){
                    Y2[con] = 255;
                }
                else{
                    Y2[con]  = abs(YX[con]) + abs(YY[con]);
                }
                Y3[con] = Y2[con];
  //             YD[con] = atan2(YY[con],YX[con])*val;
                YD[con] = (atan2(YY[con],YX[con])*val+202.5)/45;
		//printf("%f\n",YD[con]);
            }
        }
        for(int ix = 1;ix < height-1;ix++){
            for(int jx = 1;jx < width-1;jx++){
                int con=ix*width+jx;

		switch((int)YD[con]){
		case 0:
		case 4:
		case 8:
                   if(Y3[con]<=Y3[con-1] || Y3[con]<=Y3[con+1] ){
                        Y3[con] = 0;
                    }
		break;

		case 1:
		case 5:
                        if(Y3[con]<=Y3[con-width-1] || Y3[con]<=Y3[con+width+1] ){
                            Y3[con] = 0;
                        }
		break;

		case 2:
		case 6:
                        if(Y3[con]<=Y3[con-width] || Y3[con]<=Y3[con+width] ){
                            Y3[con] = 0;
                        }
		break;
		case 3:
		case 7:
                        if(Y3[con]<=Y3[con+width-1] || Y3[con]<=Y3[con-width+1] ){
                            Y3[con] = 0;
                        }
		break;
		}



            	Y4[con] = Y3[con];

            }
        }
    unsigned char out;
    int far1 = avg*0.5;
    int far2 = avg*0.9;
    int test =0;
    for(int gx=0;gx<10;gx++){
        for(int ix = 1;ix < height-1;ix++){
            for(int jx = 1;jx < width-1;jx++){
                int con=ix*width+jx;

                if(Y4[con]>far2){
                    Y4[con] = 255;
                }
                else if(Y4[con]<far1){
                    Y4[con] = 0;
                }
                else{
                    test =Y4[con-width-1] + Y4[con-width] + Y4[con-width+1] + Y4[con-1] + Y4[con+1] + Y4[con+width-1] + Y4[con+width] + Y4[con+width+1];

                    if(Y4[con-width-1] ==255 || Y4[con-width]   ==255 || Y4[con-width+1] == 255 || Y4[con-1] ==255 ||
                        Y4[con+1] == 255 || Y4[con+width-1] ==255 || Y4[con+width] ==255 || Y4[con+width+1] == 255){
                        Y4[con] = 255;
                    }
                    else if(test == 0){
                        Y4[con] = 0;
                    }
                }
                if(gx == 9){
                    if(Y4[con]<250){
                        Y4[con] =0;}
                location = (jx+200)*(vinfo.bits_per_pixel/8)+ix*finfo.line_length;
                out=Y4[ix*width+jx];
                *(fbp+location) = out;	  //R
                *(fbp+location+1) = out;  //G
                *(fbp+location+2) = out;  //B
                *(fbp+location+3) = 0;

                    

                }
            }
        }
    }


        ioctl(dev,VIDIOC_QBUF,&buf);
        clockend=clock();
        all_time+=clockend-clockbegin;
        av_time=all_time/s/CLOCKS_PER_SEC*1000;
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("\n\n\n\n\n\n\n\n\n\n\n");
        printf("av_time= %.fms\n",av_time);		//avg_time
	double fps = 1/av_time*1000;
        printf("FPS    = %.3f\n",fps);		//avg_time
        s++;
	printf("avg = %d\n",avg);
	printf("high far = %d\n",avg+25);
	printf("low far = %d\n",avg-25);

        fflush(stdout);
    }
    munmap(fbp,screensize);
	/* Stop streaming. */
    video_enable(dev, 0);
    close(dev);
    free(Y);
    free(YD);
    free(Y1);
    free(Y2);
    free(Y3);
    free(Y4);
    free(YY);
    free(YX);
    free(Y_out);
    free(Y_1_1);
    free(Y_10);
    free(Y_11);
    free(Y0_1);
    free(Y01);
    free(Y1_1);
    free(Y10);
    free(Y11);
    return 0;
}
/*
                if((YD[con] <= 22.5 && YD[con] >= -22.5) || YD[con] <= -157.5 || YD[con] >= 157.5){
                    if(Y3[con]<=Y3[con-1] || Y3[con]<=Y3[con+1] ){
                        Y3[con] = 0;
                    }
//			printf("1\n");
                }

                //Right top - Left down
                if((YD[con] <= 67.5 && YD[con] >= 22.5 )|| (YD[con] >= -157.5 && YD[con] <= -112.5 )){
                        if(Y3[con]<=Y3[con-width-1] || Y3[con]<=Y3[con+width+1] ){
                            Y3[con] = 0;
                        }
//			printf("2\n");
                }
                //Top - Down
                if((YD[con] <= 112.5 && YD[con] >= 67.5) || (YD[con] <= -67.5 && YD[con] >= -112.5 )){
                        if(Y3[con]<=Y3[con-width] || Y3[con]<=Y3[con+width] ){
                            Y3[con] = 0;
                        }
//			printf("3\n");
                }
                //Right down - Left top
                 if((YD[con] <= 157.5 && YD[con] >= 112.5) || (YD[con] <= -22.5 && YD[con] >= -67.5 )){
                        if(Y3[con]<=Y3[con+width-1] || Y3[con]<=Y3[con-width+1] ){
                            Y3[con] = 0;
                        }
//			printf("4\n");
                }


		switch((int)YD[con]){
		case 0:
		case 4:
		case 8:
                   if(Y3[con]<=Y3[con-1] || Y3[con]<=Y3[con+1] ){
                        Y3[con] = 0;
                    }
		break;

		case 1:
		case 5:
                        if(Y3[con]<=Y3[con-width-1] || Y3[con]<=Y3[con+width+1] ){
                            Y3[con] = 0;
                        }
		break;

		case 2:
		case 6:
                        if(Y3[con]<=Y3[con-width] || Y3[con]<=Y3[con+width] ){
                            Y3[con] = 0;
                        }
		break;
		case 3:
		case 7:
                        if(Y3[con]<=Y3[con+width-1] || Y3[con]<=Y3[con-width+1] ){
                            Y3[con] = 0;
                        }
		break;
		}
*/
