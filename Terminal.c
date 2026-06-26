#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "Terminal.h"


HANDLE defaultHandle;
HANDLE outputHandle;
DWORD defaultMode;
DWORD defaultOutMode;

windowInfo* currentWindow;

//returns how many columns are in the window.
void set_console_width(windowInfo* wi) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(outputHandle, &csbi)) {
        wi->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
}

windowInfo* setup_window() {
    //Input handle
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hInput, &mode);  // get current mode first

    defaultMode = mode;
    defaultHandle = hInput;

    mode &= ~ENABLE_LINE_INPUT;
    mode &= ~ENABLE_ECHO_INPUT;
    mode &= ~ENABLE_PROCESSED_INPUT;
    SetConsoleMode(hInput, mode);

    //output handle
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD outMode;
    GetConsoleMode(hOutput, &outMode);

    outputHandle = hOutput;
    defaultOutMode = outMode;

    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOutput, outMode);

    //window info object
    windowInfo* newWindow = malloc(sizeof(windowInfo));
    if(newWindow==NULL){
        return NULL;
    }
    newWindow->height = 0;
    set_console_width(newWindow);
    currentWindow = newWindow;

    return newWindow;
}



void quit_app(char* quitCode){
    if(quitCode != NULL){
        printf("\nQuit with error: %s",quitCode);
    }
    SetConsoleMode(defaultHandle,defaultMode);
    exit(0);
}
