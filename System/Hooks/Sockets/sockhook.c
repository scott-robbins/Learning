#define _GNU_SOURCE 
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <dlfcn.h>

int (*o_socket)(int,int,int);
    
int socket(int domain, int type, int protocol){
    // find the next occurrence of the socket() function
    o_socket = dlsym(RTLD_NEXT, "socket");
    
    if(o_socket == NULL){
        printf("Could not find next socket() function occurrence");
        return -1;
    }
    printf("socket constructor call intercepted\n");
    // return the result of the call to the original C socket() function
    return o_socket(domain,type,protocol);
}


// TODO: Hook Connect 

// TODO: Hook Receive

// TODO: Hook Send

/* gcc -Wall -fPIC -shared -ldl -Wl,--no-as-needed sockhook.c -o shook.so */