#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
/**
 * @brief 8-bit XOR algorithm implementation
 * taken from: https://github.com/TheAlgorithms/C
 */
uint8_t xor8(const char* s){
    uint8_t hash = 0;
    size_t i = 0;
    while (s[i] != '\0'){
        hash = (hash + s[i]) & 0xff;
        i++;
    }
    return (((hash ^ 0xff) + 1) & 0xff);
}


int main(int argc, char const *argv[]){
	if (argc > 1)
		printf("0x%x\n", xor8(argv[1]));
	else
		printf("[!!] Incorrect Usage\n");
	return 0;
}