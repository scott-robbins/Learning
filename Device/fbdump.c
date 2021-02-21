#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include <string.h>
#include <getopt.h>
#include <sys/vt.h>   /* to handle vt changing */
#include <png.h>      /* PNG lib */
#include <zlib.h>


static int srcBlue = 0;
static int srcGreen = 1;
static int srcRed = 2;
static int srcAlpha = 3;

static const int Blue = 0;
static const int Green = 1;
static const int Red = 2;
static const int Alpha = 3;

#define MAX_LEN 512
#define UNDEFINED -1

/*@noreturn@*/ static void fatal_error(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}


static void get_framebufferdata(char *device, struct fb_var_screeninfo *fb_varinfo_p, struct fb_fix_screeninfo *fb_fixedinfo, int verbose)
{
    int fd;

    /* now open framebuffer device */
    if(-1 == (fd=open(device, O_RDONLY))){
		fprintf(stderr, "Error: Couldn't open %s.\n", device);
		exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, fb_varinfo_p) != 0)
		fatal_error("ioctl FBIOGET_VSCREENINFO");

    if (ioctl(fd, FBIOGET_FSCREENINFO, fb_fixedinfo) != 0)
		fatal_error("ioctl FBIOGET_FSCREENINFO");

    if (verbose){
        fprintf(stderr, "frame buffer fixed info:\n");
        fprintf(stderr, "id: \"%s\"\n", fb_fixedinfo->id);
    	switch (fb_fixedinfo->type){
    	case FB_TYPE_PACKED_PIXELS:
    		fprintf(stderr, "type: packed pixels\n");
    		break;
    	case FB_TYPE_PLANES:
	    	fprintf(stderr, "type: non interleaved planes\n");
		    break;
    	case FB_TYPE_INTERLEAVED_PLANES:
	    	fprintf(stderr, "type: interleaved planes\n");
		    break;
    	case FB_TYPE_TEXT:
    		fprintf(stderr, "type: text/attributes\n");
            break;
    	case FB_TYPE_VGA_PLANES:
    		fprintf(stderr, "type: EGA/VGA planes\n");
		    break;
    	default:
    		fprintf(stderr, "type: undefined!\n");
	    	break;
    	}
    	// Get Framebuffer Detailed information
        fprintf(stderr, "line length: %i bytes (%i pixels)\n", fb_fixedinfo->line_length, fb_fixedinfo->line_length/(fb_varinfo_p->bits_per_pixel/8));
        fprintf(stderr, "\nframe buffer variable info:\n");
        fprintf(stderr, "resolution: %ix%i\n", fb_varinfo_p->xres, fb_varinfo_p->yres);
        fprintf(stderr, "virtual resolution: %ix%i\n", fb_varinfo_p->xres_virtual, fb_varinfo_p->yres_virtual);
        fprintf(stderr, "offset: %ix%i\n", fb_varinfo_p->xoffset, fb_varinfo_p->yoffset);
        fprintf(stderr, "bits_per_pixel: %i\n", fb_varinfo_p->bits_per_pixel);
        fprintf(stderr, "grayscale: %s\n", fb_varinfo_p->grayscale ? "true" : "false");
        fprintf(stderr, "red:   offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->red.offset, fb_varinfo_p->red.length, fb_varinfo_p->red.msb_right);
        fprintf(stderr, "green: offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->green.offset, fb_varinfo_p->green.length, fb_varinfo_p->green.msb_right);
        fprintf(stderr, "blue:  offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->blue.offset, fb_varinfo_p->blue.length, fb_varinfo_p->blue.msb_right);
        fprintf(stderr, "alpha: offset: %i, length: %i, msb_right: %i\n", fb_varinfo_p->transp.offset, fb_varinfo_p->transp.length, fb_varinfo_p->transp.msb_right);
        fprintf(stderr, "pixel format: %s\n", fb_varinfo_p->nonstd == 0 ? "standard" : "non-standard");
    }
    srcBlue = fb_varinfo_p->blue.offset >> 3;
    srcGreen = fb_varinfo_p->green.offset >> 3;
    srcRed = fb_varinfo_p->red.offset >> 3;

    if (fb_varinfo_p->transp.length > 0) {
	srcAlpha = fb_varinfo_p->transp.offset >> 3;
    } else {
	srcAlpha = -1; // not used
    }

    (void) close(fd);
}


int main(int argc, char* argvp[]){

	// Try to open the framebuffer
	char *device = "/dev/fb0";
	int fb = open(device, O_RDWR);
	if (fb <= 0)
		fatal_error("[!] Unable to open framebuffer");

	// check if framebuffer is readable
	struct fb_var_screeninfo fb_varinfo;
	struct fb_fix_screeninfo fb_fixedinfo;
	int width, height, bitdepth, line_length, skip_bytes;
	memset(&fb_varinfo, 0, sizeof(struct fb_var_screeninfo));
    memset(&fb_fixedinfo, 0, sizeof(struct fb_fix_screeninfo));

	// Load Structure to learn properties of framebuffer present 
	get_framebufferdata(device, &fb_varinfo, &fb_fixedinfo, 1);

	// Try and grab the current screen data
	size_t len = 4 * fb_varinfo.xres * fb_varinfo.yres;
	uint32_t *buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	assert(buf != MAP_FAILED);

	

}