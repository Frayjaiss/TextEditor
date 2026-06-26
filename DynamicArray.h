#ifndef DYNAMICARRAY_H_INCLUDED
#define DYNAMICARRAY_H_INCLUDED

typedef struct {
    char* data;
    int currentSize;
    int capacity;
    int maxSize;  // capacity - maxSize = buffer zone for overflow
} DynamicArray;

DynamicArray* Darray_create(int initialCapacity, int maxSize);
DynamicArray* Darray_duplicate(DynamicArray* da);
int Darray_setSize(DynamicArray* da, int newSize);
int Darray_insert(DynamicArray* da, char* word, int wordLen, int indx);
int Darray_insert_overwrite(DynamicArray* da, char* word, int wordLen, int indx);
int Darray_get(DynamicArray* da, int indx, char* out);
int Darray_remove_at(DynamicArray* da, int indx, char* out);
void Darray_del(DynamicArray* da);
void Darray_print_debug(DynamicArray* da);

#endif // DYNAMICARRAY_H_INCLUDED
