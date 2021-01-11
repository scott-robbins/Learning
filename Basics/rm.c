#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	// check args
	if (argc < 2){
		printf("Usage:$ %s <file> \n", argv[0]);
		exit(0);
	}
	if (remove(argv[1]) == 0){
		return 1;
	} else{
		printf("Unable to delete %s\n", argv[1]);
		return 0;
	}

}