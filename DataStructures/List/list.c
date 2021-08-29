#include <stdlib.h>
#include <string.h>
#include "list.h"

List* createList(){
	List* l = malloc(sizeof(List));
	l->items = calloc(DEFAULT, sizeof(void *));
    l->capacity = DEFAULT;
    return l;
}


void* append(void* item, List* l){
	// check if list can have new item added
	if(l->size >= l->capacity){
		void **newitems = // Need to re-allocate memory for new list size
            realloc(l->items, (l->capacity <<= 1) * sizeof(void **));
			free(l->items); // Free Old items
			// update items in list 
			l->items = newitems;
	}
	void * copy = memcpy(malloc(sizeof(void *)),item,sizeof(void *));
	l->items[l->size++] = copy;
	return copy;
}


void* pop(int index, List* l){
	if(!ismember(l->size, index))
		return OUT_OF_BOUNDS;
	else
		return l->items[index];
}



void delete(int index, List* l){
    if (!ismember(l->size, index))
        return;
    // remove 
    for (unsigned i = index; i < l->size; i++)
    {
        l->items[i] = l->items[i + 1];
    }
    // update list size
    l->size--;
    free(l->items[l->size]);
}

int ismember(unsigned int size, unsigned int ind){
	// make sure its a valid index 
	if (ind >=0 && ind < size)
		return 0;
	else
		return 1;
}
