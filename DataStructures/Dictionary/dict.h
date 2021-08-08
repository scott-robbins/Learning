#ifndef __DICT__H__
#define __DICT__H__
#define MAX_NUM 1500

typedef struct Dictionary{
	void* items[MAX_NUM];
	int size;

} Dictionary;


extern Dictionary* createDictionary();

extern int addLabel(Dictionary* , char [], void *);

extern void* getItem(Dictionary*, char []);

extern void destroy(Dictionary* );

#endif