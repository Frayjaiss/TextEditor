#include "DynamicArray.h"
#include "Terminal.h"
#include "TextRenderer.h"
#include <stdio.h>
#include <stdlib.h>

DynamicArray* currentString;

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

    int width = currentWindow->width;
    return col+(row*width);

}

void render_debug(){

    Darray_print_debug(currentString);
    printf(" CursorCol: %d",cursor->col);

}
int setup_renderer(){

    currentString = Darray_create(8);
    cursor = create_cursor();
    if(currentString == NULL || cursor == NULL){
        return 1;
    }
    return 0;

}


void autowrap(){



}

void render_text(int addFlag){

    printf("\033[?25l");  // hide cursor
    if(!addFlag){ //if were removing text, we want to move the cursor before we erase to the end of the screen.
        printf("\x08");
    }
    printf("\033[J");  // erase from cursor to end of screen

   // if(cursorCol > currentString->currentSize){
   //     quit_app("Cursor out of bounds, error for now");
   // }
    if(addFlag){
        //print the new character added
        char newChar = Darray_get_char(currentString, currentString->gapStartIndx-1);
        if(newChar == -1 && currentString->currentSize > 0){
            quit_app("NewChar = -1");
        }
        putchar(newChar);
    }
    if(currentString->gapStartIndx < currentString->currentSize){
        // cursor is in middle of text
        // write from gap end to end of text (everything before gap already rendered)
        int afterGapLength = currentString->currentSize - currentString->gapStartIndx;
        fwrite(currentString->data + currentString->gapEndIndx, sizeof(char), afterGapLength, stdout);
    }

    //render_debug();
    printf("\033[?25h");  // show caret
    fflush(stdout);
}

//update_cursor_pos takes a MoveCursor struct and unpacks it to move the cursor to the desired row and column
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

void store_text(char c){
    Darray_insert(currentString, c, get_linear_index(cursor->row-1,cursor->col-1));
    render_text(1);  // render before advancing cursor so cursorCol points to inserted char
    if(cursor->col >= currentWindow->width){
        MoveCursor move = {CURSOR_RELATIVE, -currentWindow->width+1, CURSOR_RELATIVE, 1,1};
        update_cursor_pos(move);
    }
    else{
        MoveCursor move = {CURSOR_RELATIVE, 1, CURSOR_RELATIVE, 0,1};
        update_cursor_pos(move);
    }
}

void remove_text(){
    if(currentString->currentSize == 0) return;
    Darray_remove_at(currentString,get_linear_index(cursor->row-1,cursor->col-1));
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
