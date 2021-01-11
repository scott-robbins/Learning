#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char text[1024];
	static int test_val = -72;

	if (argc < 2){
		printf("Usage: %s <text to print>\n", argv[0]);
		exit(0);
	}
	/* This is the vuln in part. No checks on argv[1] */
	strcpy(text, argv[1]);
	/* now use the WRONG way to print user input */
	printf(text);
	printf("\n");
	// correct way: printf("%s\n", text);
	// Show test value on the stack: 
	printf("[*] test_val at 0x%08x = %d 0x%08x\n", &test_val, test_val, test_val);

	return 0;
}