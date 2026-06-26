#include "InputHandler.h"
#include "TextRenderer.h"
#include "Terminal.h"
#include <stdio.h>

int main() {
    int setupSuccess = setup_renderer();
    windowInfo* windowSetup = setup_window();
    if(setupSuccess > 0 || windowSetup == NULL){
        quit_app("Initial application setup failed.");
    }
    bind_control_keys();
    bind_virtual_keys();
    read_input();
    quit_app(NULL);
    return 0;
}
