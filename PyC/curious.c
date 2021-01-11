/*
curious about what I can do using C/Python together.. :)
compile with: $ gcc -shared -fPIC -o lib.so curious.c
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

char* strdup(const char* org)
{
    if(org == NULL) return NULL;

    char* newstr = malloc(strlen(org)+1);
    char* p;

    if(newstr == NULL) return NULL;

    p = newstr;

    while(*org) *p++ = *org++; /* copy the string. */
    return newstr;
}

const char* get_env_address(const char* env_var, const char* prog_name){
	char* ptr = getenv(env_var);
	// adjust for program name	
	ptr += (strlen(env_var) - strlen(prog_name))*2;
	printf("%p will be at %p\n", env_var, ptr);	
	// want to figure out how to return char* of ptr
	return env_var;
}

