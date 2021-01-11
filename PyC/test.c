/*
Compile with:
$ gcc -shared -fPIC -o lib.so test.c

*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
// #include "bit.h"

void test_print(void){
	printf("C inside Python :)\n");
}

int add(int a, int b){
	return a + b;
}

void cprint(const char* str){
	printf("%s\n", str);
}

void print_unicode(const wchar_t* ustr){
	printf("%ls\n", ustr);
}

/* define a structure that will be passed from python */
struct PyStruct {
	const char* str;
	int num;
};

/* Unpack a structure passed in from python */
void show_struct(const struct PyStruct* s){
	printf("%s %d\n", s->str, s->num);
}

int get_size(const unsigned char *data){
    return sizeof(data);
}


int bit_get(const unsigned char *bitsIn, int pos){
    // allocate memory and copy over what was given
    int len = sizeof(bitsIn);
    assert(len >= 0);
    unsigned char *bits = malloc(len);
    if (bits == NULL) { abort(); }
    memcpy(&bits, &bitsIn, len);

    // now perform operation on that new memory
    unsigned char   mask;
    int             i;

    // Set mask for the bit to get
    mask = 0x80;
    for (i = 0; i < (pos % 8); i++)
        mask = mask >> 1;

    // Get the Bit
    int result = (((mask & bits[(int)(pos / 8)]) == mask) ? 1 : 0);
    return result;
}

void bit_set(unsigned char *bitsIn, int pos, int state) {
    // allocate memory and copy over what was given
    int len = sizeof(bitsIn);
    assert(len >= 0);
    unsigned char *bits = malloc(len);
    if (bits == NULL) { abort(); }
    memcpy(&bits, &bitsIn, len);

	unsigned char	mask;
	int				i;
	// set a mask for bit to get
	mask = 0x80;
	for (i = 0; i < (pos % 8); i++)
		mask = mask >> 1;
	// set the bit
	if (state)
		bits[pos / 8] = bits[pos / 8] | mask;
	else
		bits[pos / 8] = bits[pos / 8] & (~mask);

	return;

}

void bit_xor(const unsigned char *bitsIn1,
			 const unsigned char *bitsIn2,
			 unsigned char *bitsxIn, int size){
    // allocate memory and copy over what was given
    int len1 = sizeof(bitsIn1);
    assert(len1 >= 0);
    unsigned char *bits1 = malloc(len1);
    if (bits1 == NULL) { abort(); }
    memcpy(&bits1, &bitsIn1, len1);

    int len2 = sizeof(bitsIn1);
    assert(len2 >= 0);
    unsigned char *bits2 = malloc(len2);
    if (bits2 == NULL) { abort(); }
    memcpy(&bits2, &bitsIn2, len2);

    int len3 = sizeof(bitsxIn);
    assert(len3 >= 0);
    unsigned char *bitsx = malloc(len3);
    if (bitsx == NULL) { abort(); }
    memcpy(&bitsx, &bitsxIn, len3);

	int		i;
	// compute the bitwise XOR (exclusive OR) of the the buffers given
	for (i = 0; i < size; i++){
		//compare each bit of each buffer, applying XOR
		if (bit_get(bits1, i) != bit_get(bits2, i))
			bit_set(bitsx, i, 1);
		else // non matching bits are flipped
			bit_set(bitsx, i, 0);
	}

	return;
}

void bit_and(const unsigned char *bitsIn1,
             const unsigned char *bitsIn2,
             unsigned char *bitsxIn, int size){
    // allocate memory and copy over what was given
    int len1 = sizeof(bitsIn1);
    assert(len1 >= 0);
    unsigned char *bits1 = malloc(len1);
    if (bits1 == NULL) { abort(); }
    memcpy(&bits1, &bitsIn1, len1);

    int len2 = sizeof(bitsIn1);
    assert(len2 >= 0);
    unsigned char *bits2 = malloc(len2);
    if (bits2 == NULL) { abort(); }
    memcpy(&bits2, &bitsIn2, len2);

    int len3 = sizeof(bitsxIn);
    assert(len3 >= 0);
    unsigned char *bitsx = malloc(len3);
    if (bitsx == NULL) { abort(); }
    memcpy(&bitsx, &bitsxIn, len3);

    int		i;
	// compute the bitwise AND of the the buffers given
	for (i = 0; i < size; i++){
		//compare each bit of each buffer, applying AND
		if ((bit_get(bits1, i) & bit_get(bits2, i)))
			bit_set(bitsx, i, 1);
		else // set zero if both arent 1
			bit_set(bitsx, i, 0);
	}

	return;
}

void bit_or(const unsigned char *bitsIn1,
			const unsigned char *bitsIn2,
			unsigned char *bitsxIn, int size){
	// allocate memory and copy over what was given
    int len1 = sizeof(bitsIn1);
    assert(len1 >= 0);
    unsigned char *bits1 = malloc(len1);
    if (bits1 == NULL) { abort(); }
    memcpy(&bits1, &bitsIn1, len1);

    int len2 = sizeof(bitsIn1);
    assert(len2 >= 0);
    unsigned char *bits2 = malloc(len2);
    if (bits2 == NULL) { abort(); }
    memcpy(&bits2, &bitsIn2, len2);

    int len3 = sizeof(bitsxIn);
    assert(len3 >= 0);
    unsigned char *bitsx = malloc(len3);
    if (bitsx == NULL) { abort(); }
    memcpy(&bitsx, &bitsxIn, len3);

    int		i;
	// compute the bitwise OR of the the buffers given
	for (i = 0; i < size; i++){
		//compare each bit of each buffer, applying OR
		if ((bit_get(bits1, i) || bit_get(bits2, i)))
			bit_set(bitsx, i, 1);
		else // set zero if both arent 1
			bit_set(bitsx, i, 0);
	}

	return;	
}




void bit_rot_left(unsigned char *bitsIn, int size, int count) {
    // allocate memory and copy over what was given
    int len = sizeof(bitsIn);
    assert(len >= 0);
    unsigned char *bits = malloc(len);
    if (bits == NULL) { abort(); }
    memcpy(&bits, &bitsIn, len);
	// Roate input buffer to the left by specified number of bits
	int fbit, lbit, i, j;

	if (size > 0) {
		for (j = 0; j < count; j++){
			for (i = 0; i <= ((size - 1) / 8); i++){
				//get the bit to be shifted off current byte
				lbit = bit_get(&bits[i], 0);

				if (i == 0){
					//save the bit shifted off the first byte for later
					fbit = lbit;
				} else {
					// set the rightmost bit of the previous byte to the
					// leftmost bit about to be shifted off the current byte
					bit_set(&bits[i - 1], 7, lbit);
				}
				// shift current byte to the left
				bits[i] = bits[i] << 1;
			}
			// set the rightmost bit of the buffer ti the but shifted off first byte
			bit_set(bits, size - 1, fbit);
		}
	}
	return;
}

