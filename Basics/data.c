#include <stdio.h>

union DataObjA{
	long id_;
	char *name;
	char *passw;
};

struct DataObjB{
	long id_;	
	char *name;
	char *passw;
};


int main(int argc, char *argv[]){
	struct DataObjB obj1;
	union DataObjA obj2;
	printf("Struct instance is %ld bytes\n", sizeof(obj1));
	printf("Union instance is %ld bytes\n", sizeof(obj2));
	// Union holds objects in memory using a linked list, and is therefore only
	// as large as its largest member, where structs are the sumb of their members
}