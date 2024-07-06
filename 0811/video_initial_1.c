#include "inc_camera.h"


int video_open(const char *devname)
{
	struct v4l2_capability cap;
	int dev, ret;

	dev = open(devname, O_RDWR);
	if (dev < 0) {
		printf("Error opening device %s: %d.\n", devname, errno);
		return dev;
	}

	memset(&cap, 0, sizeof cap);
	ret = ioctl(dev, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		printf("Error opening device %s: unable to query device.\n",
			devname);
		close(dev);
		return ret;
	}

	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
		printf("Error opening device %s: video capture not supported.\n",
			devname);
		close(dev);
		return -EINVAL;
	}

	printf("Device %s opened: %s.\n", devname, cap.card);
	return dev;
}

int video_set_format(int dev, unsigned int w, unsigned int h, unsigned int format)
{
	struct v4l2_format fmt;
	int ret;

	//printf("video_set_format: width: %u height: %u\n");

	memset(&fmt, 0, sizeof fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = w;
	fmt.fmt.pix.height = h;
	fmt.fmt.pix.pixelformat = format;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	//fmt.fmt.pix.field = V4L2_FIELD_ANY;

	ret = ioctl(dev, VIDIOC_S_FMT, &fmt);
	if (ret < 0) {
		printf("Unable to set format: %d.\n", errno);
		return ret;
	}

	printf("Video format set: width: %u height: %u buffer size: %u\n",
		fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.sizeimage);
	return 0;
}

int video_set_framerate(int dev)
{
	struct v4l2_streamparm parm;
	int ret;

	memset(&parm, 0, sizeof parm);
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(dev, VIDIOC_G_PARM, &parm);
	if (ret < 0) {
		printf("Unable to get frame rate: %d.\n", errno);
		//return ret;
	}

	printf("Current frame rate: %u/%u\n",
		parm.parm.capture.timeperframe.numerator,
		parm.parm.capture.timeperframe.denominator);
	parm.parm.capture.timeperframe.numerator = 1;
	parm.parm.capture.timeperframe.denominator = 25;
	ret = ioctl(dev, VIDIOC_S_PARM, &parm);
	if (ret < 0) {
		printf("Unable to set frame rate: %d.\n", errno);
		//return ret;
	}

	ret = ioctl(dev, VIDIOC_G_PARM, &parm);
	if (ret < 0) {
		printf("Unable to get frame rate: %d.\n", errno);
		//return ret;
	}

	printf("Frame rate set: %u/%u\n",
		parm.parm.capture.timeperframe.numerator,
		parm.parm.capture.timeperframe.denominator);

	return 0;
}

int video_reqbufs(int dev, int nbufs)
{
	struct v4l2_requestbuffers rb;
	int ret;

	memset(&rb, 0, sizeof rb);
	rb.count = nbufs;
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(dev, VIDIOC_REQBUFS, &rb);
	if (ret < 0) {
		printf("Unable to allocate buffers: %d.\n", errno);
		return ret;
	}

	printf("%u buffers allocated.\n", rb.count);
	return rb.count;
}

int video_enable(int dev, int enable)
{
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int ret;

	ret = ioctl(dev, enable ? VIDIOC_STREAMON : VIDIOC_STREAMOFF, &type);
	if (ret < 0) {
		printf("Unable to %s capture: %d.\n",
			enable ? "start" : "stop", errno);
		return ret;
	}

	return 0;
}

int video_get_input(int dev)
{
	__u32 input;
	int ret;

	ret = ioctl(dev, VIDIOC_G_INPUT, &input);
	if (ret < 0) {
		printf("Unable to get current input: %s.\n", strerror(errno));
		return ret;
	}

	return input;
}

int video_set_input(int dev, unsigned int input)
{
	__u32 _input = input;
	int ret;

	ret = ioctl(dev, VIDIOC_S_INPUT, &_input);
	if (ret < 0)
		printf("Unable to select input %u: %s.\n", input,
			strerror(errno));

	return ret;
}

unsigned int make16color(unsigned char r, unsigned char g, unsigned char b)
{
	//return ((((r >> 3)&31)<<11) | (((g >> 2)&63)<<5) | ((b >>3 )& 31) );//RGB32//
	return (  0xff000000 | ((r << 16) & 0xff0000) | ((g << 8) & 0xff00) | ((b) & 0xff)  );//RGB32
}

int RunCommand(const char *strCommand)
{
	int iForkId, iStatus;
	iForkId = vfork();
	if (iForkId == 0)	// This is the child 
	{
		iStatus = execl("/bin/sh","sh","-c", strCommand, (char*) NULL);
				// or we will have multiple
				// mainlines running...  
	}
	else if (iForkId > 0)	// Parent, no error
	{
		iStatus = 0;
	}
	else	// Parent, with error (iForkId == -1)
	{
		iStatus = -1;
	}
	return(iStatus);
} 


int open_framebuffer(fb_v41 *vd)
{
	int fbfd,screensize;
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0) {
		printf("Error: cannot open framebuffer device.%x\n",fbfd);
		return 0;
	}
	printf("The framebuffer device was opened successfully.\n");
		
	vd->fbfd = fbfd;
	
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &vd->finfo)) {
		printf("Error reading fixed information.\n");
		return 0;
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vd->vinfo)) {
		printf("Error reading variable information.\n");
		return 0;
	}

	printf("%dx%d, %dbpp, xoffset=%d ,yoffset=%d \n", vd->vinfo.xres, vd->vinfo.yres, vd->vinfo.bits_per_pixel,vd->vinfo.xoffset,vd->vinfo.yoffset );

	screensize = vd->vinfo.xres * vd->vinfo.yres * vd->vinfo.bits_per_pixel / 8;

	vd->fbp = (unsigned int*)mmap(0,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fbfd,0);
	if ((int) vd->fbp == -1) {
		printf("Error: failed to map framebuffer device to memory.\n");
		return 0;
	}
	
	
	printf("The framebuffer device was mapped to memory successfully.\n");
	return  1;
}


int camera_init(unsigned int input,unsigned int pixelformat,unsigned int nbufs,unsigned int cam_flag,int dev,int l,int w)
{
	int ret;
	int i;
	printf("cam_flag = %d\n",cam_flag);
	video_set_input(dev, input);

	ret = video_get_input(dev);
	printf("Input %d selected\n", ret);

	/* Set the video format. */
	if (video_set_format(dev, l, w, pixelformat) < 0) 
	{
		close(dev);
		return 1;
	}
	/* Set the frame rate. */
	if (video_set_framerate(dev) < 0) 
	{
		close(dev);
		return 1;
	}
	/* Allocate buffers. */
	if ((int)(nbufs = video_reqbufs(dev, nbufs)) < 0) 
	{
		close(dev);
		return 1;
	}

	/* Map the buffers. */
	for (i = 0; i < nbufs; ++i) 
	{
		
		memset(&buf, 0, sizeof buf);
		buf.index = i;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) 
		{
			printf("Unable to query buffer %u (%d).\n", i, errno);
			close(dev);
			return 1;
		}
		printf("length: %u offset: %u\n", buf.length, buf.m.offset);
		/*
		mem[i] = mmap(0, buf.length, PROT_READ, MAP_SHARED, dev, buf.m.offset);
		if (mem[i] == MAP_FAILED) 
		{
			printf("Unable to map buffer %u (%d)\n", i, errno);
			close(dev);
			return 1;
		}
		printf("Buffer %u mapped at address %p.\n", i, mem[i]);
		*/
		switch(cam_flag)
		{
			case 0:
				/*memset(&buf, 0, sizeof buf);
				buf.index = i;
				buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf.memory = V4L2_MEMORY_MMAP;
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
				if (ret < 0) 
				{
					printf("Unable to query buffer %u (%d).\n", i, errno);
					close(dev);
					return 1;
				}
				printf("length: %u offset: %u\n", buf.length, buf.m.offset);
*/
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
				mem[i] = mmap(0, buf.length, PROT_READ, MAP_SHARED, dev, buf.m.offset);
				if (mem[i] == MAP_FAILED) 
				{
					printf("Unable to map buffer %u (%d)\n", i, errno);
					close(dev);
					return 1;
				}
				printf("Buffer %u mapped at address %p.\n", i, mem[i]);
				break;
			case 1:
				/*memset(&buf1, 0, sizeof buf1);
				buf1.index = i;
				buf1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf1.memory = V4L2_MEMORY_MMAP;
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf1);
				if (ret < 0) 
				{
					printf("Unable to query buffer %u (%d).\n", i, errno);
					close(dev);
					return 1;
				}
				printf("length: %u offset: %u\n", buf1.length, buf1.m.offset);
*/
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
				mem1[i] = mmap(0, buf.length, PROT_READ, MAP_SHARED, dev, buf.m.offset);
				if (mem1[i] == MAP_FAILED) 
				{
					printf("Unable to map buffer %u (%d)\n", i, errno);
					close(dev);
					return 1;
				}
				printf("Buffer %u mapped at address %p.\n", i, mem1[i]);
				break;
			case 2:
				/*memset(&buf2, 0, sizeof buf2);
				buf2.index = i;
				buf2.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf2.memory = V4L2_MEMORY_MMAP;
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf2);
				if (ret < 0) 
				{
					printf("Unable to query buffer %u (%d).\n", i, errno);
					close(dev);
					return 1;
				}
				printf("length: %u offset: %u\n", buf2.length, buf2.m.offset);
*/
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
				mem2[i] = mmap(0, buf.length, PROT_READ, MAP_SHARED, dev, buf.m.offset);
				if (mem2[i] == MAP_FAILED) 
				{
					printf("Unable to map buffer %u (%d)\n", i, errno);
					close(dev);
					return 1;
				}
				printf("Buffer %u mapped at address %p.\n", i, mem2[i]);
				break;
			case 3:
				/*memset(&buf3, 0, sizeof buf3);
				buf3.index = i;
				buf3.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf3.memory = V4L2_MEMORY_MMAP;
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf3);
				if (ret < 0) 
				{
					printf("Unable to query buffer %u (%d).\n", i, errno);
					close(dev);
					return 1;
				}
				printf("length: %u offset: %u\n", buf3.length, buf3.m.offset);
*/
				ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
				mem3[i] = mmap(0, buf.length, PROT_READ, MAP_SHARED, dev, buf.m.offset);
				if (mem3[i] == MAP_FAILED) 
				{
					printf("Unable to map buffer %u (%d)\n", i, errno);
					close(dev);
					return 1;
				}
				printf("Buffer %u mapped at address %p.\n", i, mem3[i]);
				break;

		}
	}

	/* Queue the buffers. */
	for (i = 0; i < nbufs; ++i) 
	{

		memset(&buf, 0, sizeof buf);
		buf.index = i;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
/*		ret = ioctl(dev, VIDIOC_QBUF, &buf);
		if (ret < 0)
		{
			printf("Unable to queue buffer (%d).\n", errno);
			close(dev);
			return 1;
		}
*/
		switch(cam_flag)
		{
			case 0:
				/*memset(&buf, 0, sizeof buf);
				buf.index = i;
				buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf.memory = V4L2_MEMORY_MMAP;*/
				ret = ioctl(dev, VIDIOC_QBUF, &buf);
				if (ret < 0)
				{
					printf("Unable to queue buffer (%d).\n", errno);
					close(dev);
					return 1;
				}
				break;
			case 1:
				/*memset(&buf1, 0, sizeof buf1);
				buf1.index = i;
				buf1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf1.memory = V4L2_MEMORY_MMAP;*/
				ret = ioctl(dev, VIDIOC_QBUF, &buf);
				if (ret < 0)
				{
					printf("Unable to queue buffer (%d).\n", errno);
					close(dev);
					return 1;
				}
				break;
			case 2:
				/*memset(&buf2, 0, sizeof buf2);
				buf2.index = i;
				buf2.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf2.memory = V4L2_MEMORY_MMAP;*/
				ret = ioctl(dev, VIDIOC_QBUF, &buf);
				if (ret < 0)
				{
					printf("Unable to queue buffer (%d).\n", errno);
					close(dev);
					return 1;
				}
				break;
			case 3:
				/*memset(&buf3, 0, sizeof buf3);
				buf3.index = i;
				buf3.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				buf3.memory = V4L2_MEMORY_MMAP;*/
				ret = ioctl(dev, VIDIOC_QBUF, &buf);
				if (ret < 0)
				{
					printf("Unable to queue buffer (%d).\n", errno);
					close(dev);
					return 1;
				}
				break;
		}
	}
	/* Start streaming. */
	video_enable(dev, 1);
	


}

