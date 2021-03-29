#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void printBits(int num){
	// compare each bit of int to determine binary value
	int nBits = 12;
	int i, j;

	
}

int main(int argc, char const *argv[]){
	// check args	
	if (argc < 2){
		printf("Usage: %s number\n", argv[0]);
		return -1;
	}
	int input = atoi(argv[1]);
	
	int mi0 = 0x01;		int mf0 = 0x0f;
	int mi1 = 0x10;		int mf1 = 0xff;

	printf("=================== SHIFTS ================\n");
	printf("0x%04x >> 1 0x%03x\t", input, input >> 1);
	printf("0x%04x << 1 0x%03x\n", input, input << 1);
	printf("0x%04x >> 2 0x%03x\t", input, input >> 2);
	printf("0x%04x << 2 0x%03x\n", input, input << 2);
	printf("0x%04x >> 3 0x%03x\t", input, input >> 3);
	printf("0x%04x << 3 0x%03x\n", input, input << 3);
	printf("======== OR ========  ======== AND ========\n");
	printf("0x%04x | 0x%02x 0x%03x\t", input, mi0, input | mi0);
	printf("0x%04x & 0x%02x 0x%03x\n", input, mi0, input & mi0);
	printf("0x%04x | 0x%02x 0x%03x\t", input, mi1, input | mi1);
	printf("0x%04x & 0x%02x 0x%03x\n", input, mi1, input & mi1);
	printf("0x%04x | 0x%02x 0x%03x\t", input, mf0, input | mf0);
	printf("0x%04x & 0x%02x 0x%03x\n", input, mf0, input & mf0);
	printf("0x%04x | 0x%02x 0x%03x\t", input, mf1, input | mf1);
	printf("0x%04x & 0x%02x 0x%03x\n", input, mf1, input & mf1);
	printf("===========================================\n");
	


	return 0;
}