#ifndef __LIST__H__
#define __LIST__H__
#define DEFAULT 1 << 4
#define OUT_OF_BOUNDS NULL

typedef struct List{
	void** items;
	unsigned int size;
	unsigned int capacity;
} List;

extern List* createList(); 			/* Function for creating a list */

extern void* append(void* item, List* l);	/* Add item to list */

extern void* pop(int index, List* l);		/* pull item from list */

extern void delete(int index, List* l);		/* remove item from list */

int ismember(unsigned int size, unsigned int index);



#endif