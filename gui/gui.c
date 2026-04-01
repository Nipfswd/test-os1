#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../video/video.h"
#include "kernel.h"
#include <stdint.h>

void kernel_main() {
    isr_install();
    irq_install();
    init_keyboard();

    video_init();

    /* Background */
    video_clear(0x003070E0);              /* kind of blue */

    /* One window */
    video_draw_window(150, 100, 500, 350,
                      0x00C0C0C0,         /* body: light gray */
                      0x00000080);        /* title: dark blue */

    /* Hang */
    while (1) {
        asm volatile("hlt");
    }
}

/* Keep user_input to satisfy existing references, even if unused now */
void user_input(char *input) {
    (void)input;
}
