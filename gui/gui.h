#ifndef GUI_H
#define GUI_H

#include <stdint.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
    const char *title;
} window_t;

/* Global flag so keyboard.c knows whether to route input to GUI */
extern int gui_active;

/* Initialize GUI and draw initial screen */
void gui_init();

/* Redraw everything */
void gui_draw();

/* Keyboard input handlers */
void gui_handle_char(char c);
void gui_handle_backspace();
void gui_handle_enter();

#endif
