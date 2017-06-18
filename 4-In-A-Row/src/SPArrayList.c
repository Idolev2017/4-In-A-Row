#include "SPArrayList.h"
#include <stdio.h>
#include <stdlib.h>
typedef SPArrayList* SPL;

SPArrayList* spArrayListCreate(int maxSize){
	if(maxSize <= 0) return NULL;
	SPArrayList* sp = (SPArrayList*) malloc(sizeof(SPArrayList));
	if(sp == NULL) {
		printMallocError();
		exit(42);
	}
	sp->actualSize = 0;
	sp->elements = 	(int*) malloc(sizeof(int)*maxSize);
	if(sp->elements == NULL) {
		printMallocError();
		free(sp);
		exit(42);
	}
	sp->maxSize = maxSize;
	return sp;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
	if(src == NULL) return NULL;
	SPArrayList* copied = spArrayListCreate(src->maxSize);
	if(copied == NULL) return NULL;
	copied->actualSize = src->actualSize;
	copied->maxSize = src ->maxSize;
	for(int i = 0; i<src->actualSize; ++i){
		(copied -> elements)[i] = (src->elements)[i];
	}
	return copied;
}

void spArrayListDestroy(SPArrayList* src){
	if(src == NULL) return;
	free(src->elements);
	free(src);
}

//check only actual size = 0 and not free the elements.
SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if (src == NULL) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	if (src == NULL || src->actualSize < index || index < 0) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == src->maxSize) return SP_ARRAY_LIST_FULL;
	for (int i = src->actualSize ; index < i ; --i){
		src->elements[i] = src->elements[i-1];
	}
	src->elements[index] = elem;
	src->actualSize += 1;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
	return spArrayListAddAt(src,elem,0);
}
SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	return spArrayListAddAt(src,elem,src->actualSize);
}
SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (src == NULL || src->actualSize <= index || index < 0) return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (src->actualSize == 0) return SP_ARRAY_LIST_EMPTY;
	for (int i = index ; i < src->actualSize-1 ; ++i){
		src->elements[i] = src->elements[i+1];
	}
	src->actualSize -= 1;
	return SP_ARRAY_LIST_SUCCESS;
}
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src,0);
}
SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src,src->actualSize-1);
}
int spArrayListGetAt(SPArrayList* src, int index){
	if(src == NULL || src->actualSize <= index || index < 0) return -1;
	return src->elements[index];
}
int spArrayListGetFirst(SPArrayList* src){
	return src->elements[0];
}
int spArrayListGetLast(SPArrayList* src){
	return src->elements[src->actualSize-1];
}
int spArrayListMaxCapacity(SPArrayList* src){
	if(src == NULL) return -1;
	return src->maxSize;
}
int spArrayListSize(SPArrayList* src){
	if(src == NULL) return -1;
	return src->actualSize;
}
bool spArrayListIsFull(SPArrayList* src){
	if(src == NULL || src->actualSize < src->maxSize) return false;
	return true;
}
bool spArrayListIsEmpty(SPArrayList* src){
	if(src == NULL || src->actualSize != 0) return false;
	return true;
}
void printMallocError(){
	printf("Error: malloc has failed");
}

