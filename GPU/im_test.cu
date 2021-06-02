// Includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
// Includes CUDA
#include <cuda_runtime.h>
// Utilities and timing functions
#include <helper_functions.h>  

#define MAX_EPSILON_ERROR 5e-3f

// Texture reference
texture<float, 2, cudaReadModeElementType> tex;

const char *imageFilename = "";


void usage(char* s){
	printf("Usage %s [image]\n", s);
}


////// Main Function //////
int main(int argc, char** argv){
	// Get Name of file to load into GPU 
	if (argc < 2){
		usage(argv[1]);
	}
	// Check Image Exists 
	char *imPath = sdkFindFilePath(argv[1], argv[0]);
	if (imPath == NULL){
		printf('[!] Cannot open %s\n', &argv[1]);
		exit(EXIT_FAILURE);
	}
	// Setup constants
	unsigned int width, height; // Image Dimensions
	float *iData = NULL; 		// Image Data 

	// Load Image into Memory
	sdkLoadPGM(argv[1], &iData, &width, &height);
	unsigned int imageSize = width * height *sizeof(float);
	printf('[*] %s Loaded\n', &argv[1]);
	printf('\t-Image Size: %d\n', imageSize);
	printf('\t-Dimensions: [%d x %d]\n', width, height);
	return 0;
}