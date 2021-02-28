#include <unistd.h>

void main(void) {
 	execvp("/bin/sh",(char *[]) { "", NULL });
} // pop a shell w execvp

