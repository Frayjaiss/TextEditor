#include "Document.h"
#include <stdlib.h>
typedef struct Row {
    struct Row* next;
    struct Row* prev;
    int isNewline;
    DynamicArray* contents;
} Row;

Row* row_create(int rowSize){

    Row* newRow = malloc(sizeof(Row));
    newRow.next = NULL;
    newRow.prev = NULL;
    newRow.contents = Darray_create(rowSize,sizeof(char));
    return newRow;

}
void row_delete(Row* row){

    Darray_del(row->contents);
    free(row);

}


Document* create_document(){

    Document* newDoc = malloc(sizeof(Document));
    //dummy node that should never be accessible
    headRow = row_create(10);
    newDoc.head = headRow;
    newDoc.tail = headRow;
    newDoc.rowCount = 0;
    newDoc.currentIndx = 1;
    newDoc.maxRowSize = currentWindow->width;
    return newDoc;

}

void document_destroy(Document* doc){
    //free all of the dynamic arrays and the rows
    Row* currentRow = doc->head;
    while(currentRow!=NULL){
        Row* nextRow = currentRow->next;
        row_delete(currentRow);
        currentRow = nextRow;
    }
    free(doc);
}
int row_insert(Row* row, char* word, int wordLen, int indx,char* overflow,int* overflowLen){
    DynamicArray* darray = row->contents;
    int insertFail = Darray_insert(row->contents,word,wordLen,indx);
    if(insertFail){
        return 1;
    }
    if(darray->currentSize > darray->maxSize){
        //start an autowrap
    }
    return '';
}

void document_insert_word(Document* doc, char* word, int wordLen, int row, int col){

    //find the closest row to the one we want to insert into from the 3 rows we track
    int headDiff = 0-row;
    int tailDiff = doc->rowCount-row;
    int recentDiff=doc->currentIndx-row;

    Row* closestRow;
    int lowestStepCount;
    if(abs(headDiff) <= abs(tailDiff) && abs(headDiff) <= abs(recentDiff)){
        closestRow = doc->head;
        lowestStepCount = headDiff;
    }
    else if(abs(tailDiff) <= abs(headDiff) && abs(tailDiff) <= abs(recentDiff)){
        closestRow = doc->tail;
        lowestStepCount = tailDiff;
    }
    else{
        closestRow = doc->currentRow;
        lowestStepCount = recentDiff;
    }
    //traverse from the closest row to the row we want
    for(int i = 0; i < abs(lowestStepCount); i++){
        if(lowestStepCount < 0){
            closestRow = closestRow->prev;
        }
        else{
            closestRow = closestRow->next;
        }
    }
    char* wordOverflow;
    int overflowLen;
    row_insert(closestRow,word,wordLen,col,&wordOverflow,&overflowLen);

}

