/** http://betteros.org/tut/graphics1.php#fbdev
 * Low-Level Graphics on Linux
 * 
 * 
 */
#include <unistd.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define CHUNK 1024

int openFrameBuffer(){
	// open frame buffer file for reading
	int fd = open("/dev/fb0",O_RDWR | O_SYNC);
	if (fd == -1)
		printf("[!] Error opening FrameBuffer\n");
	return fd;
}




uint32_t displayScreenInfo(int fbp){
	// setup framebuffer structures 
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	struct fb_cmap colormap;
	// Get variable screen information
	ioctl(fbp, FBIOGET_VSCREENINFO, &vinfo);
	// Get fixed screen information
	ioctl(fbp, FBIOGET_FSCREENINFO, &finfo);
	// Get variable screen information	
	ioctl(fbp, FBIOGET_VSCREENINFO, &vinfo);
	// Get the Colormap 
	ioctl(fbp, FBIOGETCMAP, &colormap);
	// calculate framebuffer size 
	long screensize = vinfo.yres_virtual * finfo.line_length;
	unsigned long fb_mem_offset = finfo.smem_start;
	uint32_t fb_size = finfo.smem_len;
	
	// Display information about the framebuffer
	printf("Resolution: [%dx%d]\n", vinfo.xres, vinfo.yres);
	if( vinfo.grayscale == 1) printf("FrameBuffer is Grayscale\n");
	else printf("FrameBuffer is color\n");
	printf("Bits Per Pixel: %d\n", vinfo.bits_per_pixel);
	printf("FrameBuffer Size: %ld\n", screensize);
	printf("Length of FrameBuffer Memory: %d\n", fb_size);

	// Creating a file backed memory allocation
    struct stat statbuf;
    fstat(fbp, &statbuf);

    // Map framebuffer into memory
    // void *frameBuff = mmap((void *) fb_mem_offset, fb_size, PROT_READ,MAP_SHARED, fbp, 0);
    // if (frameBuff == MAP_FAILED) printf("[!] Failed to Map FrameBuffer into Memory\n");
    // // When finished unmap the memory 
    // int err = munmap((void *) frameBuff, fb_size);
    return finfo.smem_len;
}

int readFrameBuffer(uint32_t buffSize){
	char buf[CHUNK];
	size_t nread;
	
	FILE *fcopy;
	FILE* fbp = fopen("/dev/fb0", "r");
	fcopy = fopen("screenshot", "w");

	if (fcopy){
		while((nread = fread(buf, 1, sizeof buf, fbp)) > 0)
			fwrite(buf, 1, nread, fcopy);
		if (ferror(fcopy)) {
			// deal with error 
			printf("Error reading framebuffer");
		}
		// close the copy;
		fclose(fcopy);
		fclose(fbp);
	}
}



int main(int argc, char* argv[]){
	int fb = openFrameBuffer();
	uint32_t fbsize = displayScreenInfo(fb);
	close(fb);
	readFrameBuffer(fbsize);
	
	
	
	return 0;
}