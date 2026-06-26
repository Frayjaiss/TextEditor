#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED
#include "DynamicArray.h"

//this is a double linked list. Row is a node, Document is the list.

typedef struct Row Row;

typedef struct {
    Row* head;
    Row* tail;
    Row* currentRow;
    int maxRowSize;
    int currentIndx;
    int rowCount;
} Document;

Document* create_document();
void document_destroy(Document* doc);
void document_insert_word(Document* doc, Word* w, int row, int col);
void document_delete_char(Document* doc, int row, int col);
void document_print(Document* doc);
int document_row_count(Document* doc);


#endif // DOCUMENT_H_INCLUDED
