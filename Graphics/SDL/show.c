#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

// gcc cube.c -lSDL2 -O3 -o box



typedef struct      s_pixel{
	int             x;
	int             y;
}                   t_pixel;

typedef struct cell{
	int x;
	int y;
	int value;
} t_cell;

int fread_int(FILE* in);
int fread_byte(FILE* in);
void read_bmp_head(int* wptr, int* hptr, FILE* in);
void read_color(int* rptr, int* gptr, int* bptr, FILE* in);

bool error_sdl(char* err_msg){
	printf("[!] %s-> SDSL_Error: %s\n", err_msg, SDL_GetError());
	return false;
}

bool init_sdl(SDL_Window** window, SDL_Renderer** renderer, int WIDTH, int HEIGHT){
	// setup components one by one
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return (error_sdl("Failed to initialize SDL"));
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0 ,window, renderer);
	// If this fails window will be null pointer (like a file operation)
	if (*window == NULL)
		return (error_sdl("Unable to create Window"));
	// Clear window to empty screen
	SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
	SDL_RenderClear(*renderer);
	// Set cursor to a purplish color
	SDL_SetRenderDrawColor(*renderer, 0, 0xff, 0, 0);
	return true;
}

int fread_int(FILE* in) {
  int ans;
  int check = fread(&ans, sizeof(int), 1, in);
  assert (check == 1);
  return ans;
}

int fread_byte(FILE* in) {
  unsigned char ans;
  int check = fread(&ans, sizeof(char), 1, in);
  assert (check == 1);
  return ans;
};

void read_bmp_head(int* wptr, int* hptr, FILE* in) {
  	// variables for important image parameters
  	int fhlen = 14;
  	int ihlen = 40;
  	int fsize;
  	int temp;

  	// read file header
  	temp = fread_byte(in); // magic number part 1
  	assert (temp == 'B');
  	temp = fread_byte(in); // magic number part 2
  	assert (temp == 'M');
  	fsize = fread_int(in); // file size
  	fread_int(in); // unused 4 bytes
  	temp = fread_int(in); // pixels offset
  	assert (temp == fhlen + ihlen);

  	// read image header
  	temp = fread_int(in); // image header length
  	assert (temp == ihlen);
  	*wptr = fread_int(in); // image width
  	*hptr = fread_int(in); // image height
  	temp = fread_int(in); // 1 image plane, 24 bits per pixel
  	assert (temp == 1 + (24 << 16));
  	temp = fread_int(in); // compression type
  	assert (temp == 0);
  	fread_int(in); // image size (ignored)
  	fread_int(in); // horiz resolution (ignored)
  	fread_int(in); // vertical resolution (ignored)
  	temp = fread_int(in); // color map entries used
  	assert (temp == 0);
  	fread_int(in); // # of "important" colors (ignored)

  	// check file size
  	int rowlen = *wptr * 3;
  	int pixlen = *hptr * rowlen;
  	assert (fsize == fhlen + ihlen + pixlen);
}

void read_color(int* rptr, int* gptr, int* bptr, FILE* in) {
  // note: pixels are in reverse order BGR
  *bptr = fread_byte(in);
  *gptr = fread_byte(in);
  *rptr = fread_byte(in);
}


void draw_image(SDL_Renderer *renderer, FILE*fp, int width, int height){
	// read and write pixel data
	for (int ycoord=height; ycoord >= 1; --ycoord) {
		for (int xcoord=0; xcoord < width; ++xcoord) {
      		int r, g, b;
      		read_color(&r, &g, &b, fp);
      		SDL_SetRenderDrawColor(renderer, r, g, b, 0);
      		SDL_RenderDrawPoint(renderer, xcoord, ycoord);
      	}
    }
}




int main(int argc, char*argv[]){
	// Create Window 
	SDL_Event eve; 
	SDL_Window *win;
	SDL_Renderer *ren;

	
	
	// Check args
	if (argc > 1){
		// try to load  in argument as image file
		FILE* fp = fopen(argv[1], "rb");
		if (!fp && strstr(argv[1],".bmp")){
			printf("[x] Unable to open %s\n", argv[1]);
			return 1;
		}
		int width;
		int height;
		read_bmp_head(&width, &height, fp);
		#define WINDOW_WIDTH    width
		#define WINDOW_HEIGHT   height
		if(!(init_sdl(&win, &ren, width, height)))
			return 1;
		int counter = WINDOW_WIDTH;
		printf("Image is [%d,%d]\n", width, height);
		draw_image(ren, fp, width, height);
		SDL_RenderPresent(ren);
	}else{
		#define WINDOW_WIDTH	600
		#define WINDOW_HEIGHT	600
		if(!(init_sdl(&win, &ren, WINDOW_WIDTH, WINDOW_HEIGHT)))
			return 1;
		SDL_RenderPresent(ren);
	}

	// Let it run
	while (42){
		// Check for events
		if (SDL_PollEvent(&eve)){
			
			// check if user is quitting
			if (eve.type == SDL_QUIT)
				break;

		}
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}