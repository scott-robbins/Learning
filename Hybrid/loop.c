/* 
Taken From:
https://blog.holbertonschool.com/hack-the-virtual-memory-c-strings-proc/
compile with:
gcc -Wall -Wextra -pedantic -Werror loop.c -o loop 
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**              
 * main - uses strdup to create a new string, loops forever-ever              
 * Return: EXIT_FAILURE if malloc failed. Other never returns
 */
int main(void)
{
     char *s;
     unsigned long int i;

     s = strdup("EXAMPLE");
     if (s == NULL)
     {
          fprintf(stderr, "Can't allocate mem with malloc\n");
          return (EXIT_FAILURE);
     }
     i = 0;
     while (s)
     {
          printf("[%lu]\t%s (%p)\n", i, s, (void *)s);
          sleep(1.5);
          i++;
     }
     return (EXIT_SUCCESS);
}