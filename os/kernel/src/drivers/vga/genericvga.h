#ifndef GENERICVGA_H
#define GENERICVGA_H

#include <stdint.h>
#include "drivers/dal/videocard/videocard.h"

void generic_vga_driver_init();
int16_t generic_vga_set_video_mode(uint16_t mode);
uint8_t generic_vga_is_text_mode();
video_mode *generic_vga_get_current_video_mode();

int8_t generic_vga_draw_pixel_external_buffer(uint8_t* buffer, uint16_t mode, int8_t color, uint16_t x, uint16_t y);
int8_t generic_vga_draw_line_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t generic_vga_draw_circle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t x, uint16_t y, uint16_t radius);
int8_t generic_vga_draw_rectangle_external_buffer(uint8_t* buffer, uint16_t mode, uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by);
int8_t generic_vga_clear_screen_external_buffer(uint8_t* buffer, uint16_t mode, uint16_t* x, uint16_t* y);

int8_t generic_vga_swap_external_buffer(uint8_t* buffer, uint16_t mode);
uint8_t* generic_vga_create_external_buffer(uint16_t mode);
void generic_vga_destroy_external_buffer(uint8_t* buffer);

#endif