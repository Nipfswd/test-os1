#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

typedef struct {
    uint16_t attributes;
    uint8_t  winA;
    uint8_t  winB;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segmentA;
    uint16_t segmentB;
    uint32_t realFctPtr;
    uint16_t pitch;
    uint16_t Xres;
    uint16_t Yres;
    uint8_t  Wchar;
    uint8_t  Ychar;
    uint8_t  planes;
    uint8_t  bpp;
    uint8_t  banks;
    uint8_t  memory_model;
    uint8_t  bank_size;
    uint8_t  image_pages;
    uint8_t  reserved0;

    uint8_t  red_mask;
    uint8_t  red_position;
    uint8_t  green_mask;
    uint8_t  green_position;
    uint8_t  blue_mask;
    uint8_t  blue_position;
    uint8_t  rsv_mask;
    uint8_t  rsv_position;
    uint8_t  directcolor_attributes;

    uint32_t physbase;
    uint32_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint16_t reserved4;
} __attribute__((packed)) vbe_mode_info_t;

void video_init();
void video_clear(uint32_t color);
void video_putpixel(int x, int y, uint32_t color);
void video_fill_rect(int x, int y, int w, int h, uint32_t color);
void video_draw_window(int x, int y, int w, int h, uint32_t body, uint32_t title);

#endif
