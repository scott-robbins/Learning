#include <inttypes.h>
#include <stdio.h>

/**
 * brief 32-bit CRC algorithm implementation
 * taken from: https://github.com/TheAlgorithms/C
 */
uint32_t crc32(const char* s){
    uint32_t crc = 0xffffffff;
    size_t i = 0;
    // Iterate until reaching end of string (NULL terminated)
    while (s[i] != '\0'){ 
        uint8_t byte = s[i]; // grab the 8 bytes of current char
        crc = crc ^ byte;    // crc XOR with current byte
        for (uint8_t j = 8; j > 0; --j) //magic?
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        i++;
    }
    return crc ^ 0xffffffff;
}

int main(int argc, char const *argv[]){
    if (argc > 1)
        printf("%" PRIu32 "\n", crc32(argv[1]));
    else
        printf("[!!] Incorrect Usage\n");
    return 0;
}