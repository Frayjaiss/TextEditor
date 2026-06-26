#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include <windows.h>

#define BUFFER_SIZE 32

typedef struct{

int width;
int height;

}windowInfo;

extern windowInfo* currentWindow;
extern HANDLE defaultHandle;
extern DWORD defaultMode;

windowInfo* setup_window();
void quit_app(char* quitCode);

#endif // TERMINAL_H_INCLUDED
