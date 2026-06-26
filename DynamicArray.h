#ifndef DYNAMICARRAY_H_INCLUDED
#define DYNAMICARRAY_H_INCLUDED

typedef struct{

    char* data;
    int currentSize;
    int capacity;
    int gapInitialSize;
    //inclusive
    int gapStartIndx;
    //not inclusive
    int gapEndIndx;

}DynamicArray;

DynamicArray* Darray_create(int initialCapacity);

DynamicArray* Darray_duplicate(DynamicArray* da);

int Darray_setSize(DynamicArray*da, int newSize);

int Darray_insert(DynamicArray* da, char c,int indx);

char Darray_pop_back(DynamicArray* da);

void Darray_del(DynamicArray* da);

char Darray_get_char(DynamicArray* da, int indx);

int Darray_interpret_logical_indx_as_physical(DynamicArray* da,int indx);

int Darray_move_gap(DynamicArray* da,int indx);

int Darray_resize_gap(DynamicArray* da,int gapStart);

char Darray_remove_at(DynamicArray* da,int indx);

void Darray_print_debug(DynamicArray* da);
#endif // DYNAMICARRAY_H_INCLUDED
