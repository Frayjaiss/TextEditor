#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "DynamicArray.h"
#include "Terminal.h"

static int Darray_resize(DynamicArray* da);

DynamicArray* Darray_create(int initialCapacity, int maxSize){
    DynamicArray* da = malloc(sizeof(DynamicArray));
    if(da == NULL) return NULL;

    // allocate double the maxSize to create buffer zone for overflow
    da->data = malloc(initialCapacity * 2);
    if(da->data == NULL){
        free(da);
        return NULL;
    }
    da->currentSize = 0;
    da->maxSize = maxSize;
    da->capacity = initialCapacity * 2;
    return da;
}

DynamicArray* Darray_duplicate(DynamicArray* da){
    if(da == NULL) return NULL;

    DynamicArray* da2 = malloc(sizeof(DynamicArray));
    if(da2 == NULL) return NULL;

    da2->data = malloc(da->capacity);
    if(da2->data == NULL){
        free(da2);
        return NULL;
    }
    memcpy(da2->data, da->data, da->capacity);
    da2->capacity = da->capacity;
    da2->maxSize = da->maxSize;
    da2->currentSize = da->currentSize;
    return da2;
}

static int Darray_resize(DynamicArray* da){
    int newCapacity = da->capacity * 2;
    char* newData = realloc(da->data, newCapacity);
    if(newData == NULL) return 1;
    da->data = newData;
    da->capacity = newCapacity;
    return 0;
}

int Darray_setSize(DynamicArray* da, int newSize){
    if(newSize < da->currentSize) return 1;
    char* newData = realloc(da->data, newSize);
    if(newData == NULL) return 2;
    da->data = newData;
    da->capacity = newSize;
    return 0;
}

int Darray_insert(DynamicArray* da, char* word, int wordLen, int indx){
    if(indx < 0 || indx > da->currentSize) return 1;
    // shift everything right by wordLen to make room
    memmove(da->data + indx + wordLen, da->data + indx, da->currentSize - indx);
    // copy word into the gap
    memcpy(da->data + indx, word, wordLen);
    da->currentSize += wordLen;
    return 0;
}

int Darray_insert_overwrite(DynamicArray* da, char* word, int wordLen, int indx){
    if(indx < 0 || indx + wordLen > da->currentSize) return 1;
    memcpy(da->data + indx, word, wordLen);
    return 0;
}

int Darray_get(DynamicArray* da, int indx, char* out){
    if(indx < 0 || indx >= da->currentSize) return 1;
    *out = da->data[indx];
    return 0;
}

int Darray_remove_at(DynamicArray* da, int indx, char* out){
    if(indx < 0 || indx >= da->currentSize) return 1;
    if(out != NULL){
        *out = da->data[indx];
    }
    // shift everything left by 1
    memmove(da->data + indx, da->data + indx + 1, da->currentSize - indx - 1);
    da->currentSize--;
    return 0;
}

void Darray_del(DynamicArray* da){
    free(da->data);
    free(da);
}

void Darray_print_debug(DynamicArray* da){
    printf("\nArray metadata: currentSize=%d | maxSize=%d | capacity=%d\n",
        da->currentSize, da->maxSize, da->capacity);
    for(int i = 0; i < da->currentSize; i++){
        printf("[%c]", da->data[i]);
    }
    if(da->currentSize < da->capacity){
        printf(" buffer: ");
        for(int i = da->currentSize; i < da->capacity; i++){
            printf("[%d]", da->data[i]);  // print as int to see buffer values
        }
    }
    printf("\n");
}






