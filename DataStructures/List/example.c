#include "list.h"
#include <stdio.h>

void main(){
	List* example = createList();
	printf("[-] Created List\n");
	
	// Add ints to list 
	for(int i=0;i<3; i++){
		append(&i, example);
		int size = (int) example->size;
		printf("%d added to list [Size:%d]\n", i, size);
	}

	// Display items in list 
	for(unsigned int i=0; i < example->size; i++){
		void* val = example->items[i];
		printf("Arr[%d] = %d\n", i, *(int *)val);
	}

	
	// Clean up the list 
	printf("[-] Deleting items from list\n");
	for(int j=0; j < example->size; j++){
		delete(&j, example);
	}
	printf("-list size: %d\n", example->size);

}