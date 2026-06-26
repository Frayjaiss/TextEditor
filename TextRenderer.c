#include "DynamicArray.h"
#include "Terminal.h"
#include "TextRenderer.h"
#include <stdio.h>
#include <stdlib.h>

DynamicArray* currentString;

//an array that holds information about the rows of text
DynamicArray* rowCounts;

Cursor* cursor;

Cursor* create_cursor(){
    Cursor* c = malloc(sizeof(Cursor));
    if(c==NULL){
        return NULL;
    }
    c->row = 1;
    c->col = 1;
    return c;
}

int get_linear_index(int row, int col){

    int rowOffset = 0;
    for(int i = 0; i < row-1;i++){
        if(row > rowCounts->currentSize){
            quit_app("indexing by row failed, row > rowCounts->currentSize");
        }
        rowInfo rowObj;
        Darray_get(rowCounts, i, &rowObj);
        rowOffset += rowObj.rowSize;
    }
    return col+rowOffset;

}

void render_debug(){

    Darray_print_debug(currentString);
    printf(" CursorCol: %d",cursor->col);

}
int setup_renderer(){

    currentString = Darray_create(8, sizeof(char));
    rowCounts = Darray_create(8, sizeof(rowInfo));
    cursor = create_cursor();
    if(currentString == NULL || cursor == NULL || rowCounts == NULL){
        return 1;
    }
    int startVal = 0;
    Darray_insert(rowCounts,&startVal,0);
    return 0;

}


void row_change(int row){

    int rowStart = get_linear_index(row,0);


}

//inserts a newline behind the segmented word so it prints on the same line
int autowrap(int newestCharIndx){
    int nearestSpace = newestCharIndx;
    //find where the nearest space is to isolate the word that should wrap down
    char currentChar = 0;
    Darray_get(currentString, nearestSpace, &currentChar);
    while(currentChar != ' '){
        //printf("\nindex: %d, char: %d\n", newestCharIndx, Darray_get_char(currentString, nearestSpace));
        nearestSpace--;
        int result = Darray_get(currentString, nearestSpace, &currentChar);
        if(result < 0){
            return -1; //if this ever happens, we tried to autowrap from behind the top row. Something went wrong, just quit out.
        }
    }
    //tquit_app("breakpoint 1");
    char newline = '\n';
    Darray_insert(currentString, &newline, nearestSpace+1);
    //return how many characters that moved to the new row
    return newestCharIndx - nearestSpace;
}

void render_text(int addFlag){

    printf("\033[?25l");  // hide cursor
    if(!addFlag){ //if were removing text, we want to move the cursor before we erase to the end of the screen.
        printf("\x08");
    }
    printf("\033[J");  // erase from cursor to end of screen

    if(addFlag){
        //print the new character added
        char newChar = 0;
        Darray_get(currentString, currentString->gapStartIndx-1, &newChar);
        if(newChar == -1 && currentString->currentSize > 0){
            quit_app("NewChar = -1");
        }
        putchar(newChar);
        if(newChar == '\n'){
            MoveCursor move = {CURSOR_RELATIVE, -currentWindow->width+1, CURSOR_RELATIVE, 0,1};
            update_cursor_pos(move);
        }
    }
    if(currentString->gapStartIndx < currentString->currentSize){
        // cursor is in middle of text
        // write from gap end to end of text (everything before gap already rendered)
        int afterGapLength = currentString->currentSize - currentString->gapStartIndx;
        fwrite((char*)currentString->data + currentString->gapEndIndx, sizeof(char), afterGapLength, stdout);
    }

    render_debug();
    printf("\033[?25h");  // show caret
    fflush(stdout);
}

//update_cursor_pog takes a MoveCursor struct and unpacks it to move the cursor to the desired row and column
void update_cursor_pos(MoveCursor currentMove){
    if(currentMove.colType == CURSOR_ABSOLUTE){
        cursor->col = currentMove.colVal;
    }
    else{
        cursor->col += currentMove.colVal;
    }
    if(currentMove.rowType == CURSOR_ABSOLUTE){
        cursor->row = currentMove.rowVal;
    }
    else{
        cursor->row += currentMove.rowVal;
    }
    if(currentMove.physicalMoveFlag == 1){
        printf("\033[%d;%dH", cursor->row, cursor->col);
    }
}


int count_line(int indx){
    //if the line
    int charCount = 0;
    Darray_get(rowCounts,indx,&charCount);
    return charCount;

}

//takes the index of the line in rowCounts and will recalculate the line size by counting
//from rowCounts[indx].rowStartIndx to the first \n character.
int update_line_count(int indx){
    rowInfo currentRow;
    Darray_get(rowCounts,indx,&currentRow);
    int currentIndx = currentRow.rowStartIndx;
    //step forward in the array until we read a \n character
    char currentChar = '0';
    Darray_get(currentString,currentIndx,&currentChar);
    while(currentChar != '\n'){
        currentIndx++;
        Darray_get(currentString,currentIndx,&currentChar);
        if(currentIndx > currentWindow->width){
            quit_app("new line not found, quit app in TextRenderer.update_line_count");
        }
    }
    currentRow.rowSize = currentIndx;
    Darray_insert_overwrite(rowCounts,&currentRow,indx);
}

void store_text(char c){
    int indx = get_linear_index(cursor->row-1,cursor->col-1);
    int autowrapTextMoved = 0;
    Darray_insert(currentString, &c, indx);
    if(cursor->col >= currentWindow->width-1){
        int newEofL = autowrap(indx);
        if(newEofL < 0){
            quit_app("autowrap failed");
        }
        MoveCursor move = {CURSOR_RELATIVE, -newEofL, CURSOR_RELATIVE, 0,1};
        update_cursor_pos(move);
        autowrapTextMoved = newEofL;
        update_line_count(cursor->row-1);
    }
    render_text(1);  // render before advancing cursor so cursorCol points to inserted char
    MoveCursor move = {CURSOR_RELATIVE, 1+autowrapTextMoved, CURSOR_RELATIVE, 0,1};
    update_cursor_pos(move);
    //if(autowrapTriggeredFlag){ //if we autowrap, were inserting a newline character into the array
        //int moveFail = Darray_move_gap(currentString,Darray_interpret_logical_indx_as_physical(currentString,indx+2));
        //if(moveFail){
            //printf("\nquit with code %d",moveFail);
            //quit_app("move failed");
       // }
    //}
}

void remove_text(){
    if(currentString->currentSize == 0) return;
    Darray_remove_at(currentString, get_linear_index(cursor->row-1,cursor->col-1), NULL);
    render_text(0);
    if(cursor->col != 1){
        MoveCursor move = {CURSOR_RELATIVE, -1, CURSOR_RELATIVE, 0,1};
        update_cursor_pos(move);
    }
    if(cursor->col <= 1 && cursor->row > 1){
        MoveCursor move = {CURSOR_RELATIVE, currentWindow->width, CURSOR_RELATIVE, -1,1};
        update_cursor_pos(move);
    }

}
