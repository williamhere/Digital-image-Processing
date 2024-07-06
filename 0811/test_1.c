
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

int main()
{
 float val = 180 / PI;
 float ans;
 int x;
 int y;
 int a =  0;
 int b =  0;
 int c =  0;
 int d =  0;
 int e =  0;
 int f =  0;
 int g =  0;
 int h =  0;
 int i =  0;


  printf("%2d  %2d  %2d\n",a,b,c);
  printf("%2d  %2d  %2d\n",d,e,f);
  printf("%2d  %2d  %2d\n",g,h,i);

y = a+b*2+c-g-h*2-i;
x = a+d*2+g-c-f*2-i;
 ans = atan2(y,x)*val;
   printf("x = %2d\n",x);
     printf("y = %2d\n",y);
  printf("%f",ans);
  system("pause");
    return 0;
}
