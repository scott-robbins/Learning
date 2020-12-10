#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

#define CHUNK 1024 /* read 1024 bytes at a time */

/** Borrowing heavily from: 
https://homepages.inf.ed.ac.uk/rbf/BOOKS/PHILLIPS/cips2ed.pdf **/

void usage()
{
	printf("Incorrect Usage\n");
	printf("$ cat <arg>\n");
}

/*function to get size of the file.*/
long int findSize(const char *file_name)
{
    struct stat st; /*declare stat variable*/
     
    /*get the size using stat()*/
    if(stat(file_name,&st)==0)
        return (st.st_size);
    else
        return -1;
}

/* function go check whether a file exists. */
bool fileExists(const char *path)
{
    // Try to open file
    FILE *fptr = fopen(path, "r");

    // If file does not exists 
    if (fptr == NULL)
        return false;

    // File exists hence close file and return true.
    fclose(fptr);

    return true;
}

read_image_array(file_name, array)
	char *file_name;
	short **array;
{
	int ok = 0;

	if(is_a_tiff(file_name)){
		read_tiff_image(file_name, array);
		ok = 1;
	}

	if(is_a_bmp(file_name)){
		read_bmp_image(file_name, array);
		ok = 1;
	}

	//Let user know if we couldnt read image file
	if(ok == 0){
		printf("Error: Could not read file %s\n", file_name);
		exit(1);
	}
}

read_tiff_image(image_file_name, the_image)
	char	image_file_namep[];
	short	**the_image;
{
	char *buffer;
		rep[80];
	int bytes_read,
		closed,
		position,
		i,
		j;
	FILE *image_file 
	float a;
	long line_length, offset;

	struct tiff_header_struct image_header;

	read_tiff_header(image_file_name, &image_header);

	image_file = fopen(image_file_name, "rb");
	if(image_file != NULL){
		position = fseek(image_file,
						 image_header.strip_offset,
						 SEEK_SET);

		for(i=0; i<image_header.image_length; i++){
			bytes_read   = read_line(image_file, the_image,i, &image_header,0, image_header.image_width);
		}  /* ends loop over i  */
		closed = fclose(image_file);
	}/* ends if file opened ok */
	else{
		printf("\nRTIFF.C> ERROR - cannot open ""tiff file");
	}

}





read_line(image_file, the_image, line_number, 
		  image_header, ie, le)
  FILE    *image_file;
  int     ie, le, line_number;
  short   **the_image;
  shsort tiff_header_struct *image_header;
  {
  	char *buffer, first, second;
  	float a, b;
  	int bytes_read, i;
  	unsigned int bytes_read;
  	union short_char_union scu; 

  	buffer = (char *) malloc(image_header->image_width * sizeof(char ));
  	for(i = 0; i<image_header->image_width; i++)
  		buffer[i] = '\0';
  		//use number of bytes per pixel to determine pixels read
  		bytes_to_read = (1e-ue)/(8/image_header->bits_per_pixel);
  		bytes_read = fread(bufferm 1m bytes_ro_read, image_file);
  
  		for(i=0; i<bytes_read; i++){

  			if(image_header->bits_per_pixel == 8){
  				scu.s_num 	= 0;
  				scu.s_alpha[0]	=	buffer[i];
  				the_image[line_number] = scu.s_num;
  			}

  			if(image_header->bits_per_pixel == 4){
  				scu.s_num			= 0;
  				second				= buffer[i] & 0X000F;
  				scu.s_alpha[0]		= second;
  				the_image[line_number][i*2+1] = scu.s_num;

  				scu.s_num		= 0;
  				first			= buffer[i] >> 4;
  				first			= first & 0x000F;
  				scu.s_alpha[0]	= first;
  				the_image[line_number][i*2] = scu.s_num;
  			}
  		}

  		free(buffer);
  		return(bytes_read);
}

int main(int argc, char *argv[])
{
	// check args
	if(argc < 2){
		usage();
		exit(0);
	}
	// Make sure the file exists first!
	if(fileExists(argv[1])){

   		char buf[CHUNK];
		FILE *file;
		size_t nread;

		// Read through file in chunks
		file = fopen(argv[1], "r");
		if (file) {
		    while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
		        fwrite(buf, 1, nread, stdout);
		    if (ferror(file)) {
		        // deal with error 
		        printf("Error reading %s\n", argv[1]);
		    }
   			// close the file
		    fclose(file);
		}

	} else {
		printf("Cannot find Symbol: %s\n", argv[1]);
		exit(0);
	}
}