#include <ctype.h>
#include <stdio.h>
#include "Terminal.h"
#include "InputHandler.h"
#include "TextRenderer.h"

Command controlBinds[32] = {0};
Command virtualBinds[256] = {0};


//VIRTUAL FUNC DEFINITIONS:
void backspace(){
    remove_text();
}
void enter(){
    store_text('\n');
    MoveCursor move = {CURSOR_ABSOLUTE, 1, CURSOR_RELATIVE, 1,1};
    update_cursor_pos(move);
}
void tab(){
    store_text('\t');
}
//shared implementation for moving the caret
void move_caret(int direction) {
    MoveCursor move = {CURSOR_RELATIVE, 0, CURSOR_RELATIVE, 0, 1};

    switch(direction) {
        case VK_LEFT:move.colVal = -1; break;
        case VK_UP:move.rowVal = -1; break;
        case VK_RIGHT:move.colVal = 1; break;
        case VK_DOWN:move.rowVal = 1; break;
    }

    update_cursor_pos(move);
}

//wrappers for arrow keys
void arrow_left(){move_caret(VK_LEFT);}
void arrow_right(){move_caret(VK_RIGHT);}
void arrow_up(){move_caret(VK_UP);}
void arrow_down(){move_caret(VK_DOWN);}

//COMMAND FUNC DEFINITIONS:

void quit(){
    quit_app("Normal quit");
}

//LOGIC FUNCTIONS

void bind_virtual_keys(){

    virtualBinds[VK_BACK] = backspace;
    virtualBinds[VK_RETURN] = enter;
    virtualBinds[VK_TAB] = tab;
    virtualBinds[VK_LEFT] = arrow_left;
    virtualBinds[VK_RIGHT] = arrow_right;
    virtualBinds[VK_UP] = arrow_up;
    virtualBinds[VK_DOWN] = arrow_down;

}

void bind_control_keys(){

    controlBinds[CTRL_KEY('q')] = quit;

}


void handle_control_keys(char c){

    Command ctrlCmnd = controlBinds[(int)c];
    if(ctrlCmnd != NULL){
        ctrlCmnd();
    }

}

void handle_virtual_keys(int vc){

    Command vCmnd = virtualBinds[vc];
    if(vCmnd != NULL){
        vCmnd();
    }

}

void handle_key_input(INPUT_RECORD record){
    if (record.Event.KeyEvent.bKeyDown) {  // only care about presses not releases
        char c = record.Event.KeyEvent.uChar.AsciiChar;
        int vc = record.Event.KeyEvent.wVirtualKeyCode;
        handle_virtual_keys(vc);
        if (iscntrl(c)) {
            handle_control_keys(c);
        }
        else{
            store_text(c);
        }
    }
}

void read_input(){
    INPUT_RECORD buffer[BUFFER_SIZE];
    DWORD eventsToRead;
    while(1){
        ReadConsoleInput(defaultHandle,buffer,BUFFER_SIZE,&eventsToRead); // set the value of buffer to the first value found in conhost.exe's buffer.
        for(DWORD i = 0; i < eventsToRead; i++){
            INPUT_RECORD record = buffer[i];
            switch (record.EventType){
                case KEY_EVENT:
                    handle_key_input(record);
                    break;
                case MOUSE_EVENT:
                    break;
                default:
                    break;
            }
        }
    }
}

