#ifndef INPUTHANLDER_H_INCLUDED
#define INPUTHANLDER_H_INCLUDED
#include <windows.h>

#define CTRL_KEY(k) ((k) & 0x1F)
typedef void (*Command)();
//function pointers to any bound ctrl+key
extern Command controlBinds[32];
//function pointers to windows virtual keys (backspace, enter, tab...)
extern Command virtualBinds[256];

// VIRTUAL BINDS:
void backspace();

// COMMAND BINDS:

void quit();

// LOGIC FLOW:
void bind_virtual_keys();
void bind_control_keys();

void handle_key_input(INPUT_RECORD record);

void read_input();
#endif // INPUTHANLDER_H_INCLUDED
