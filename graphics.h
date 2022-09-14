/* Friedrich Doku frd20 */
#ifndef GRAPHICS_H
#define GRAPHICS_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <termios.h>
#include <sys/select.h>
#include "iso_font.h"

struct color {
    union {
    	uint16_t val;
		struct {
			uint16_t blue : 5;
			uint16_t green : 6;
			uint16_t red : 5;
		};
	};
} __attribute__((packed));

typedef struct color color_t;

color_t* create_color(uint16_t r, uint16_t g, uint16_t b);

void init_graphics();

void exit_graphics();

char get_key();

void sleep_ms(long ms);

void clear_screen(void* img);

void draw_pixel(void* img, int x, int y, color_t color);

void draw_line(void* img, int x1, int y1, int x2, int y2, color_t c);

void draw_text(void* img, int x, int y, const char* text, color_t c);

void* new_offscreen_buffer();

void blit(void* src);

#endif
