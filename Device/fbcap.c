#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/vt.h>      
#include <assert.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <zlib.h>
#include <time.h>
#include <png.h> 
#include <err.h>


static int sR = 2; static const int R = 2;
static int sG = 1; static const int G = 1;
static int sB = 0; static const int B = 0;
static int sA = 3; static const int A = 3;

#define MAXBUF	512

#define UNDEFINED -1

int VERBOSE = 1; // On for debugging

void fatal_error(char *message){
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

static void showFramebufferInfo(char *device, struct fb_var_screeninfo *fb_varinfo_p, struct fb_fix_screeninfo *fb_fixedinfo){
	int fd;	
	// open framebuffer device 
	if (-1 == (fd = open(device, O_RDONLY))){ fatal_error("[!] Cannot open /dev/fb0\n"); }
    
    // Fill Structs with framebuffer info using ioctl call defined by kernel
    if (ioctl(fd, FBIOGET_VSCREENINFO, fb_varinfo_p) != 0){ fatal_error("ioctl FBIOGET_VSCREENINFO");}
    if (ioctl(fd, FBIOGET_VSCREENINFO, fb_varinfo_p) != 0){ fatal_error("ioctl FBIOGET_VSCREENINFO");}

    // show pixel format type 
    printf("[*] Framebuffer Information:\n");
    switch (fb_fixedinfo->type){
    	case FB_TYPE_PACKED_PIXELS:
    		fprintf(stderr, "- uses packed pixels\n");
    		break;
    	case FB_TYPE_PLANES:
	    	fprintf(stderr, "- uses non interleaved planes\n");
		    break;
    	case FB_TYPE_INTERLEAVED_PLANES:
	    	fprintf(stderr, "- uses interleaved planes\n");
		    break;
    	case FB_TYPE_TEXT:
    		fprintf(stderr, "- uses text/attributes\n");
            break;
    	case FB_TYPE_VGA_PLANES:
    		fprintf(stderr, "- uses EGA/VGA planes\n");
		    break;
    	default:
    		fprintf(stderr, "- uses an unknown format!\n");
	    	break;
    	}
    	// Get Framebuffer Detailed information
        printf("- line length: %i bytes (%i pixels)\n", fb_fixedinfo->line_length, fb_fixedinfo->line_length/(fb_varinfo_p->bits_per_pixel/8));
        printf("- resolution: %ix%i\n", fb_varinfo_p->xres, fb_varinfo_p->yres);
        printf("- virtual resolution: %ix%i\n", fb_varinfo_p->xres_virtual, fb_varinfo_p->yres_virtual);
        printf("- offset: %ix%i\n", fb_varinfo_p->xoffset, fb_varinfo_p->yoffset);
        printf("- bits_per_pixel: %i\n", fb_varinfo_p->bits_per_pixel);
        printf("- grayscale: %s\n", fb_varinfo_p->grayscale ? "true" : "false");
        printf("- red:   offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->red.offset, fb_varinfo_p->red.length, fb_varinfo_p->red.msb_right);
        printf("- green: offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->green.offset, fb_varinfo_p->green.length, fb_varinfo_p->green.msb_right);
        printf("- blue:  offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->blue.offset, fb_varinfo_p->blue.length, fb_varinfo_p->blue.msb_right);
        printf("- alpha: offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->transp.offset, fb_varinfo_p->transp.length, fb_varinfo_p->transp.msb_right);
        printf("- pixel format: %s\n", fb_varinfo_p->nonstd == 0 ? "standard" : "non-standard");
}


void mysterious_bit_twiddles(struct fb_var_screeninfo *fb_varinfo_p){
	sR = fb_varinfo_p->red.offset >> 3;
	sG = fb_varinfo_p->green.offset >> 3;
	sB = fb_varinfo_p->blue.offset >> 3;
	if (fb_varinfo_p->transp.offset > 0){
		sA = fb_varinfo_p->transp.offset >> 3;
	} else {
		sA = -1; // unused
	}
}


int main(int argc, char const *argv[]){
	// check for arguments 
	if (argc >= 1 && (strcmp(argv[1],"-v")==0)){
		VERBOSE = 1;
	}

	if (argc >= 2 && (strcmp(argv[1], "-o")==0)){
		char const *screencapfile = argv[2];
	} else{
		char const *screencapfile = "screenshot.png";
	}

	// Check if we have permission to access the frame buffer 
	char * dev = "/dev/fb0";
	int fb = open(dev, O_RDWR);
	if (fb <= 0)
		fatal_error("[!] Unable to open framebuffer. Are you root?");

	// Setup structure for storing data about the framebuffer
	struct fb_var_screeninfo fb_varinfo;
	struct fb_fix_screeninfo fb_fixedinfo;
	int width, height, bitdepth, line_length, skip_bytes;
	memset(&fb_varinfo, 0, sizeof(struct fb_var_screeninfo));
    memset(&fb_fixedinfo, 0, sizeof(struct fb_fix_screeninfo));

    // If verbose show framebuffer information 
    if (VERBOSE == 1){showFramebufferInfo(dev, &fb_varinfo, &fb_fixedinfo);}
    // Not fully sure what these bit twiddles do, something with mapping colors
	// but its in the bits of code I'm working off of 
	mysterious_bit_twiddles(&fb_varinfo);
	// load dimensions of screen, bitdepth and line length
	bitdepth = (int) fb_varinfo.bits_per_pixel;
    width = (int) fb_varinfo.xres;
    height = (int) fb_varinfo.yres;
    line_length = (int) fb_fixedinfo.line_length/(fb_varinfo.bits_per_pixel>>3);
    
    // I think this is calculating unused bits after shifting for R/G/B
    skip_bytes =  (fb_varinfo.yoffset * fb_varinfo.xres) * (fb_varinfo.bits_per_pixel >> 3);

    // Allocate Memory for Framebuffer Image Data
    size_t buf_size;
    unsigned char *buf_p;
    buf_size = line_length * height * (((unsigned int) bitdepth + 7) >> 3);
    buf_p = (unsigned char*) malloc(buf_size);
	memset(buf_p, 0, buf_size);

	// Grab the pixels in the frame data to construct into image
	if (VERBOSE == 1){ printf("[*] Capturing Screenshot from /dev/fb0\n");}

	size_t len = 4 * fb_varinfo.xres * fb_varinfo.yres;
	uint32_t *buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	
	// Jumpt to offset in the frame buffer 
    if (skip_bytes)
        lseek(fb, skip_bytes, SEEK_SET);

    // check whether there is memory to read
    if (buf_p == NULL || read(fb, buf_p, buf_size) != (ssize_t) buf_size)
		fatal_error("[!] Error: Not enough memory or data\n");
	
	// Create a buffer to hold converted image buffer data 
	size_t imagebuffer = (size_t) line_length * height * 4;
	unsigned char *imageoutbuff = (unsigned char*) malloc(buf_size);
	memset(imageoutbuff, 0, buf_size);

	printf("[*] Converting Image to %d bits\n", bitdepth);

	// Dump Data to a file 

	FILE* scfp = fopen(screencapfile, "w");
	if (scfp == NULL)
		fatal_error("[!] Unable to create screenshot file");
	printf("[*] Saving Screenshot to %s\n", screencapfile);
	



	// Free Memory when Finished!
	free(buf_p);
	close(scfp);
	return 0; // All was good, return 0
}