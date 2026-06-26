#include <stdlib.h>
#include <string.h>
#include "DynamicArray.h"
#include "Terminal.h"
#include <stdio.h>

//0 indicates no errors

static int Darray_resize(DynamicArray* da);


DynamicArray* Darray_create(int initialCapacity){

    DynamicArray* da = malloc(sizeof(DynamicArray));
    if(da==NULL){
        return NULL;
    }
    da->data = malloc(initialCapacity*sizeof(char));
    if(da->data == NULL){
        free(da);
        return NULL;
    }
    da->currentSize = 0;
    da->capacity = initialCapacity;
    da->gapInitialSize = 10;
    da->gapStartIndx = 0;
    da->gapEndIndx = 0;
    int fail = Darray_move_gap(da,0);
    if(fail){
        quit_app("creating initial gap failed");
    }
    int fail2 = Darray_resize_gap(da,0);
    return da;

}

DynamicArray* Darray_duplicate(DynamicArray* da){

    if(da==NULL){
        return NULL;
    }

    DynamicArray* da2 = malloc(sizeof(DynamicArray));
    if(da2==NULL){
        return NULL;
    }
    da2->data = malloc(da->capacity * sizeof(char));
    memcpy(da2->data, da->data, da->capacity);
    if(da2->data == NULL){
        free(da2);
        return NULL;
    }
    da2->capacity = da->capacity;
    da2->currentSize = da->currentSize;
    da2->gapStartIndx = da->gapStartIndx;
    da2->gapEndIndx = da->gapEndIndx;
    da2->gapInitialSize = da->gapInitialSize;
    memcpy(da2->data, da->data, da->currentSize * sizeof(char));

    return da2;

}

static int Darray_resize(DynamicArray*da){
    int newCapacity = da->capacity*2;
    char* newData = realloc(da->data,newCapacity*sizeof(char));
    if(newData == NULL){
        return 1; // allocation failed, return error and do not overwrite
    }
    da->data = newData;
    da->capacity = newCapacity;
    return 0;
}


int Darray_setSize(DynamicArray*da, int newSize){

    if(newSize < da->currentSize){
        return 1; //allocation failed, new size is too small
    }

    char* newData = realloc(da->data,newSize*sizeof(char));
    if(newData == NULL){
        return 2; // allocation failed, return error and do not overwrite
    }
    da->data = newData;
    da->capacity = newSize;
    return 0;

}


char Darray_pop_back(DynamicArray* da) {
    if (da->currentSize == 0) {
        return -1;  // nothing to remove
    }
    char backChar = da->data[da->currentSize-1];
    da->currentSize--;
    return backChar;
}

// this function takes the logical index the user requested and returns the physical index for it
int Darray_interpret_logical_indx_as_physical(DynamicArray* da, int indx){
    if(indx <  da->gapStartIndx){
        return indx;
    }
    return indx + (da->gapEndIndx - da->gapStartIndx);
}


int Darray_resize_gap(DynamicArray* da, int gapStart){
    // keep resizing until we have room for the gap
    while(da->currentSize + da->gapInitialSize > da->capacity){
        int resizeError = Darray_resize(da);
        if(resizeError) return 1;
    }
    // shift everything from end of array rightward to make room for new gap
    for(int i = da->currentSize + (da->gapEndIndx - da->gapStartIndx) - 1; i >= da->gapEndIndx; i--){
        da->data[i + da->gapInitialSize] = da->data[i];
    }
    da->gapStartIndx = gapStart;
    da->gapEndIndx = gapStart + da->gapInitialSize;
    return 0;
}


int Darray_move_gap(DynamicArray* da, int indx){
    int oldGapSize = da->gapEndIndx - da->gapStartIndx;
    if(indx > da->currentSize+oldGapSize || indx < 0){
        return 1;
    }
    // if indx is in gap, dont move the gap
    if(indx >= da->gapStartIndx && indx < da->gapEndIndx){
        return 1;
    }
    if(indx > da->gapStartIndx){
        // moving right: shift characters left to fill old gap
        for(int i = da->gapStartIndx; (i + oldGapSize) < indx; i++){
            da->data[i] = da->data[i + oldGapSize];
            da->data[i + oldGapSize] = 0;  // zero out old positions
        }
        da->gapStartIndx = indx - oldGapSize;
        da->gapEndIndx = indx;
    } else {
        // moving left: shift characters right to fill old gap
        for(int i = da->gapStartIndx - 1; i >= indx; i--){
            da->data[i + oldGapSize] = da->data[i];
            da->data[i] = 0;  // zero out old positions
        }
        da->gapStartIndx = indx;
        da->gapEndIndx = indx + oldGapSize;
    }
    return 0;
}

int Darray_insert(DynamicArray* da, char c, int indx){
    // if the cursor has moved to after the gap, translate to physical index.
    if(indx > da->gapStartIndx){
        indx = Darray_interpret_logical_indx_as_physical(da, indx);
    }

    // check if write is happening outside the gap
    if(indx < da->gapStartIndx || indx >= da->gapEndIndx){
        //printf("%d||%d",physIndx < da->gapStartIndx,physIndx >= da->gapEndIndx);
        Darray_move_gap(da, indx);
    }

    da->data[da->gapStartIndx] = c;
    da->gapStartIndx++;
    da->currentSize++;

    // if gap exhausted, make a new one
    if(da->gapStartIndx == da->gapEndIndx){
        int resizeErr = Darray_resize_gap(da, da->gapStartIndx);
        if(resizeErr){
            quit_app("Gap resize failed");
        }
    }
    return 0;
}
void Darray_del(DynamicArray* da){
    free(da->data);
    free(da);
}
char Darray_get_char(DynamicArray* da, int indx){
    if(indx < 0 || indx >= da->currentSize){
        return -1;
    }
    indx = Darray_interpret_logical_indx_as_physical(da, indx);
    return da->data[indx];
}

char Darray_remove_at(DynamicArray* da,int indx){

    if(da->gapStartIndx == 0){
        return -1;
    }
    // if the cursor has moved to after the gap, translate to physical index.
    if(indx > da->gapStartIndx){
        indx = Darray_interpret_logical_indx_as_physical(da, indx);
    }

    // check if write is happening outside the gap
    if(indx < da->gapStartIndx || indx >= da->gapEndIndx){
        //printf("%d||%d",physIndx < da->gapStartIndx,physIndx >= da->gapEndIndx);
        Darray_move_gap(da, indx);
    }


    char removedChar = da->data[da->gapStartIndx-1];
    //erase the character at the start
    da->data[da->gapStartIndx-1] = 0;
    da->gapStartIndx--;
    da->currentSize--;
    return removedChar;

}


void Darray_print_debug(DynamicArray* da){

    printf("\n");
    printf("Array metadata: CurrentSize: %d | GapStart: %d | GapEnd: %d | Capacity: %d\n",da->currentSize,da->gapStartIndx,da->gapEndIndx,da->capacity);
    int gapSize = da->gapEndIndx-da->gapStartIndx;
    for(int i=0;i<(da->currentSize+gapSize);i++){
        putchar('[');
        if(i>=da->gapStartIndx && i<da->gapEndIndx){
            putchar('-');
        }
        else{
            char currentChar = da->data[i];
            putchar(currentChar);
        }
        printf("],");
    }
}
