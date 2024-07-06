main_file = main_framebuffer_neon.c
execute_file = SoC_lab

processor = cortex-a57
floating = neon-vfpv4
ABI = hard
neno_envir = -mcpu=$(processor) -mfpu=$(floating) -mfloat-abi=$(ABI)

CC = arm-linux-gnueabihf-gcc
AR = arm-linux-gnueabihf-ar

all:
	$(CC) -O3 $(neno_envir) -ffast-math -c video_initial.c
	$(AR) rcs libcam.a video_initial.o
	$(CC) -static -O3 $(neno_envir) -ffast-math $(main_file) \
	      -finline-functions -D_FILE_OFFSET_BITS=64 -lpthread -L. -lcam -lm -ffast-math -o $(execute_file)

clean:
	@rm -vf $(execute_file) *.o
