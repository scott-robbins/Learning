#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	// check args
	if (argc < 3){
		printf("Usage: %s number1 number2\n", argv[0]);
		return -1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int result = a * b;
	printf("%d x %d = %d\n", a, b, result);

	// Use bitwise right shift to divide number by power of 2
    printf("%d / (2^1) = %d\n",a, (a >> 1));
    // Use bitwise left shift to multiply  number by power of 2
    printf("12 * (2^1) = %d\n", (a << 1));
    // Check even odd
    if (b & 1)
        printf("%d is an ODD number.\n", b);
    else
        printf("%d is an EVEN number.\n", b);

    // Make all bits off.
    unsigned char flag = 0;

    // Set marital status YES, i.e. 0th bit 1
    // (flag => 0000 0001 = 1)
    flag = flag | 1;

    // Set voting status YES, i.e. 1st bit 1 
    // (flag => 0000 0011 = 3)
    flag = flag | 2;

    // Set VISA eligibility status YES, i.e. 2nd bit 1
    // (flag => 0000 0111 = 7)
    flag = flag | 4;    

    // Print flag value 
    printf("flag, DECIMAL = %d, HEX = %x\n\n", flag, flag);


    // Unset or set all flags to false.
    flag = flag & (~(1 << 0));
    flag = flag & (~(1 << 1));
    flag = flag & (~(1 << 2));

    // Print flag value
    printf("\nflag, DECIMAL = %d, HEX = %x\n", flag, flag);

     // Convert to lowercase
    printf("Bitwise Lower:  'a' => '%c'\n", ('a' | ' '));
    printf("Bitwise Lower: 'A' => '%c'\n", ('A' | ' '));

    // Convert to uppercase
    printf("Bitwise Upper: 'a' => '%c'\n", ('a' & '_'));
    printf("Bitwise Upper: 'A' => '%c'\n", ('a' & '_'));

    int min = (b ^ (a ^ b) & -(a < b));
    int max = (a ^ (a ^ b) & -(a < b));

    printf("Minimum(10, 20) => %d\n", min);
    printf("Maximum(10, 20) => %d\n", max);

}