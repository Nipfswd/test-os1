#include "gui.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

int gui_active = 0;

static window_t main_window;
static char input_buffer[128];

static void putc_at(char c, int x, int y) {
    char s[2] = { c, '\0' };
    kprint_at(s, x, y);
}

static void draw_hline(int x, int y, int w, char c) {
    for (int i = 0; i < w; i++) {
        putc_at(c, x + i, y);
    }
}

static void draw_vline(int x, int y, int h, char c) {
    for (int i = 0; i < h; i++) {
        putc_at(c, x, y + i);
    }
}

static void draw_window(window_t *win) {
    int x = win->x;
    int y = win->y;
    int w = win->w;
    int h = win->h;

    /* Corners */
    putc_at('+', x,         y);
    putc_at('+', x + w - 1, y);
    putc_at('+', x,         y + h - 1);
    putc_at('+', x + w - 1, y + h - 1);

    /* Borders */
    draw_hline(x + 1,     y,         w - 2, '-');
    draw_hline(x + 1,     y + h - 1, w - 2, '-');
    draw_vline(x,         y + 1,     h - 2, '|');
    draw_vline(x + w - 1, y + 1,     h - 2, '|');

    /* Title */
    if (win->title) {
        kprint_at(" ", x + 2, y);
        kprint_at((char*)win->title, x + 3, y);
        kprint_at(" ", x + 3 + (int)strlen((char*)win->title), y);
    }
}

static void draw_input_line() {
    int x = main_window.x + 2;
    int y = main_window.y + main_window.h - 2;

    /* Clear line */
    for (int i = 0; i < main_window.w - 4; i++) {
        putc_at(' ', x + i, y);
    }

    kprint_at("> ", x, y);
    kprint_at(input_buffer, x + 2, y);
}

void gui_draw() {
    clear_screen();
    draw_window(&main_window);

    kprint_at("NaznaOS GUI Demo", main_window.x + 2, main_window.y + 2);
    kprint_at("Type text and press Enter.", main_window.x + 2, main_window.y + 3);

    draw_input_line();
}

void gui_init() {
    memory_set((uint8_t*)input_buffer, 0, sizeof(input_buffer));

    main_window.x = 10;
    main_window.y = 4;
    main_window.w = 60;
    main_window.h = 15;
    main_window.title = "NaznaOS";

    gui_active = 1;
    gui_draw();
}

void gui_handle_char(char c) {
    int len = (int)strlen(input_buffer);
    if (len < (int)sizeof(input_buffer) - 1 && c >= 32 && c <= 126) {
        input_buffer[len] = c;
        input_buffer[len + 1] = '\0';
    }
    draw_input_line();
}

void gui_handle_backspace() {
    int len = (int)strlen(input_buffer);
    if (len > 0) {
        input_buffer[len - 1] = '\0';
    }
    draw_input_line();
}

void gui_handle_enter() {
    int x = main_window.x + 2;
    int y = main_window.y + 5;

    /* Clear output line */
    for (int i = 0; i < main_window.w - 4; i++) {
        putc_at(' ', x + i, y);
    }

    kprint_at("You typed: ", x, y);
    kprint_at(input_buffer, x + 11, y);

    memory_set((uint8_t*)input_buffer, 0, sizeof(input_buffer));
    draw_input_line();
}
