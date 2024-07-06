#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/fb.h>
#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <pthread.h>

typedef struct  _fb_v4l
{
        int fbfd ;                                                                     
	unsigned int *fbp;
        struct fb_var_screeninfo vinfo;                                                 
        struct fb_fix_screeninfo finfo;                                              
}fb_v41;

fb_v41 vd;
//---------------------------預定義參數宣告---------------------------------------
#define lsize 1280
#define wsize 720
//#define y_size video_width*video_height																		//frame y_size 720*480
//#define uv_size (video_width*video_height)>>2	
#define V4L_BUFFERS_DEFAULT	4
#define V4L_BUFFERS_MAX		32
void *mem[V4L_BUFFERS_MAX];
void *mem1[V4L_BUFFERS_MAX];
void *mem2[V4L_BUFFERS_MAX];
void *mem3[V4L_BUFFERS_MAX];
unsigned int redblue_rbg_out[1920*1080*2];
static int *y_list,*g_u_list,*r_v_list,*g_v_list,*b_u_list;
static unsigned char *input_y,*input_u,*input_v,*BT656_buffer,*yuv,*BT656_buffer1,*yuv1,*BT656_buffer2,*yuv2,*BT656_buffer3,*yuv3,*BT656_buffer_bsd_left,*BT656_buffer_bsd_right,*yuv_left;

struct v4l2_buffer buf;
//struct v4l2_buffer buf1;
//struct v4l2_buffer buf2;
//struct v4l2_buffer buf3;
//---------------------------函數宣告---------------------------------------
extern void uyvy2rgb(int width, int height, unsigned char *src, unsigned char *dst);
extern void BY_yuvlist(int *y_list,int *g_u_list,int *g_v_list,int *r_v_list,int * b_u_list);
extern int video_open(const char *devname);
extern int video_set_format(int dev, unsigned int w, unsigned int h, unsigned int format);
extern int video_reqbufs(int dev, int nbufs);
extern int video_enable(int dev, int enable);
extern int video_get_input(int dev);
extern int video_set_input(int dev, unsigned int input);
extern unsigned int make16color(unsigned char r, unsigned char g, unsigned char b);
extern int RunCommand(const char *strCommand);
extern int open_framebuffer(fb_v41 *vd);
extern int camera_init(unsigned int input,unsigned int pixelformat,unsigned int nbufs,unsigned int cam_flag,int dev,int l,int w);
extern void yuyv2rgb(int width, int height, unsigned char *src, unsigned int *dst);


