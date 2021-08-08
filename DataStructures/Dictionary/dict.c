#include <stdlib.h>
#include <stdio.h>
#include "dict.h"

/* method for creating unique indexes of items */
int hash(char key[]){
	unsigned int code = 0;
	for(int i=0; key[i] != '\0'; i++)
		code = key[i] + (code << 6) + (code << 16) - code;
	return code % MAX_NUM;
}


/* Define method for intializing a dictionary structure */
Dictionary* createDictionary(){
	Dictionary* d = malloc(sizeof(Dictionary));
	if (!d){
		printf("[!] Unable to create Dictionary\n");
	}else{
		d->size = 0;
		for(int i=0; i<MAX_NUM; i++)
			d->items[i] = NULL;	
	}
	return d;
}

/* Add new item:value pair to dictioanry */
int addLabel(Dictionary* d, char key[], void* value){
	// key of each item is their hash
	unsigned int index = hash(key);
	
	// check that index is valid 
	if (index<0 || index > MAX_NUM){
		return -1;
	}else{
		d->items[index] = value;
		return 0;
	}
}

/* Retrieve item from dictionary by it's label */
void* getItem(Dictionary* d, char label[]){
	// determine what the index should be for this label
	int index = hash(label);

	// check if this element exists
	if(d->items[index]){
		return d->items[index];
	}
	printf("[!] Invalid Label: %s\n", label);
	return NULL;
}

/* Destructor for dictionary data structure*/
void destroy(Dictionary* d){
	for(int i=0; i < d->size; i++){
		free(d->items[i]);
	}
	free(d);
}