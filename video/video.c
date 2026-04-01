#include "video.h"
#include <stdint.h>

#define VBE_MODE_INFO_ADDR 0x8000

static vbe_mode_info_t *vbe = (vbe_mode_info_t*)VBE_MODE_INFO_ADDR;

static uint8_t  *fb_base   = 0;
static uint32_t  fb_width  = 0;
static uint32_t  fb_height = 0;
static uint32_t  fb_pitch  = 0;
static uint32_t  fb_bpp    = 0;

void video_init() {
    fb_base   = (uint8_t*)(uintptr_t)vbe->physbase;
    fb_width  = vbe->Xres;
    fb_height = vbe->Yres;
    fb_pitch  = vbe->pitch;
    fb_bpp    = vbe->bpp;
}

void video_putpixel(int x, int y, uint32_t color) {
    if (!fb_base) return;
    if (x < 0 || y < 0) return;
    if ((uint32_t)x >= fb_width || (uint32_t)y >= fb_height) return;

    uint32_t bytes_per_pixel = fb_bpp / 8;
    uint8_t *row = fb_base + y * fb_pitch;
    uint8_t *p   = row + x * bytes_per_pixel;

    /* Assume 32bpp XRGB/ARGB */
    p[0] = (uint8_t)(color & 0xFF);
    p[1] = (uint8_t)((color >> 8) & 0xFF);
    p[2] = (uint8_t)((color >> 16) & 0xFF);
    p[3] = (uint8_t)((color >> 24) & 0xFF);
}

void video_clear(uint32_t color) {
    if (!fb_base) return;
    for (uint32_t y = 0; y < fb_height; y++) {
        for (uint32_t x = 0; x < fb_width; x++) {
            video_putpixel(x, y, color);
        }
    }
}

void video_fill_rect(int x, int y, int w, int h, uint32_t color) {
    for (int j = 0; j < h; j++) {
        int yy = y + j;
        if (yy < 0 || (uint32_t)yy >= fb_height) continue;
        for (int i = 0; i < w; i++) {
            int xx = x + i;
            if (xx < 0 || (uint32_t)xx >= fb_width) continue;
            video_putpixel(xx, yy, color);
        }
    }
}

void video_draw_window(int x, int y, int w, int h, uint32_t body, uint32_t title) {
    if (w <= 0 || h <= 0) return;

    /* Body */
    video_fill_rect(x, y, w, h, body);

    /* Title bar (top 24 px) */
    int title_h = 24;
    if (title_h > h) title_h = h;
    video_fill_rect(x, y, w, title_h, title);

    /* Border */
    uint32_t border = 0x00000000;
    for (int i = 0; i < w; i++) {
        video_putpixel(x + i, y,         border);
        video_putpixel(x + i, y + h - 1, border);
    }
    for (int j = 0; j < h; j++) {
        video_putpixel(x,         y + j, border);
        video_putpixel(x + w - 1, y + j, border);
    }
}
