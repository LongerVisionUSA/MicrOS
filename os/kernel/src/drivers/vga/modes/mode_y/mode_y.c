#include "mode_Y.h"
#include "drivers/vga/modes/registerFunctions.h"
#include "memory/heap/heap.h"
#include "drivers/dal/videocard/videocard.h"

//REGISTER VALUES
unsigned char g_320x200x256_modey[] =
	{
	/* MISC */
		0x63,
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00, 0x00
	};

//PALETTE
unsigned char paletteY[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 
    0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 
    0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 
    0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x08, 0x08, 0x08, 0x0B, 0x0B, 0x0B, 0x0E, 0x0E, 0x0E, 0x11, 
    0x11, 0x11, 0x14, 0x14, 0x14, 0x18, 0x18, 0x18, 0x1C, 0x1C, 0x1C, 0x20, 0x20, 0x20, 0x24, 0x24, 
    0x24, 0x28, 0x28, 0x28, 0x2D, 0x2D, 0x2D, 0x32, 0x32, 0x32, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F, 
    0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00, 0x3F, 0x3F, 0x00, 0x3F, 0x3F, 
    0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 
    0x00, 0x3F, 0x2F, 0x00, 0x3F, 0x3F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x00, 
    0x00, 0x3F, 0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00, 0x3F, 0x3F, 0x00, 
    0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 
    0x3F, 0x37, 0x1F, 0x3F, 0x3F, 0x1F, 0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 
    0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F, 0x37, 0x1F, 0x3F, 0x3F, 0x1F, 0x37, 
    0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 0x3F, 0x1F, 0x1F, 0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 
    0x2F, 0x1F, 0x3F, 0x37, 0x1F, 0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 0x3F, 
    0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0x3F, 0x2D, 0x3F, 0x3F, 
    0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 
    0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0x3F, 0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D, 
    0x2D, 0x3F, 0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0x3F, 0x2D, 
    0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 0x0E, 0x00, 
    0x1C, 0x15, 0x00, 0x1C, 0x1C, 0x00, 0x1C, 0x1C, 0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 
    0x1C, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 0x1C, 0x1C, 0x00, 0x15, 
    0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 
    0x0E, 0x00, 0x1C, 0x15, 0x00, 0x1C, 0x1C, 0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C, 
    0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E, 0x1C, 0x1C, 0x0E, 0x1C, 0x1C, 
    0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 
    0x0E, 0x1C, 0x18, 0x0E, 0x1C, 0x1C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x0E, 
    0x0E, 0x1C, 0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E, 0x1C, 0x1C, 0x0E, 
    0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14, 
    0x1C, 0x1A, 0x14, 0x1C, 0x1C, 0x14, 0x1C, 0x1C, 0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 
    0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14, 0x1C, 0x1A, 0x14, 0x1C, 0x1C, 0x14, 0x1A, 
    0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 0x1C, 0x14, 0x14, 0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 
    0x18, 0x14, 0x1C, 0x1A, 0x14, 0x1C, 0x1C, 0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 0x1C, 
    0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 0x10, 0x00, 0x10, 0x10, 
    0x00, 0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 
    0x00, 0x10, 0x0C, 0x00, 0x10, 0x10, 0x00, 0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00, 
    0x00, 0x10, 0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 0x10, 0x00, 
    0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 0x0C, 0x08, 
    0x10, 0x0E, 0x08, 0x10, 0x10, 0x08, 0x10, 0x10, 0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 
    0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 0x10, 0x10, 0x08, 0x0E, 
    0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10, 0x08, 0x08, 0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 
    0x0C, 0x08, 0x10, 0x0E, 0x08, 0x10, 0x10, 0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10, 
    0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B, 0x10, 0x10, 0x0B, 0x10, 0x10, 
    0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 
    0x0B, 0x10, 0x0F, 0x0B, 0x10, 0x10, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x0B, 
    0x0B, 0x10, 0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B, 0x10, 0x10, 0x0B, 
    0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//DOUBLE BUFFER POINTER;

unsigned char *MODEY_BUFFER[] = {NULL, NULL, NULL, NULL};
unsigned char bufferTurnedOnY = 0;

int8_t setModeY()
{
    writeRegisters(g_320x200x256_modey);
    set_vga_palette(paletteY);
    clearScreenY();
    video_card_set_turn_on_buffer_func(&turnOnBufferY);
    video_card_set_turn_off_buffer_func(&turnOffBufferY);
    video_card_set_is_buffer_on_func(&isBufferOnY);
    video_card_set_swap_buffers_func(&swapBuffersY);
    video_card_set_draw_pixel_func(&drawPixelY);
    video_card_set_draw_line_func(&drawLineY);
    video_card_set_draw_circle_func(&drawCircleY);
    video_card_set_draw_rectangle_func(&drawRectangleY);
    video_card_set_clear_screen_func(&clearScreenY);
    return 0x69;
}

int8_t turnOnBufferY()
{
    if(bufferTurnedOnY) return -1;
    for(int i = 3; i >= 0; i--)
    {
        MODEY_BUFFER[i] = heap_kernel_alloc(MODEY_HEIGHT * MODEY_WIDTH / 4, 0);
        if(MODEY_BUFFER[i] == NULL)
        {
            for(int j = 3; j >= i; j--)
            {
                heap_kernel_dealloc(MODEY_BUFFER[j]);
                MODEY_BUFFER[j] = NULL;
            }
            return -1;
        }
    }
    video_card_set_draw_pixel_func(&drawPixelYBuffered);
    video_card_set_draw_line_func(&drawLineYBuffered);
    video_card_set_draw_circle_func(&drawCircleYBuffered);
    video_card_set_draw_rectangle_func(&drawRectangleYBuffered);
    video_card_set_clear_screen_func(&clearScreenYBuffered);
    bufferTurnedOnY = 1;
    return 0;
}

int8_t turnOffBufferY()
{
    if(!bufferTurnedOnY) return -1;
    for(int i = 3; i >= 0; i--)
    {
        heap_kernel_dealloc(MODEY_BUFFER[i]);
        MODEY_BUFFER[i] = NULL;
    }
    video_card_set_draw_pixel_func(&drawPixelY);
    video_card_set_draw_line_func(&drawLineY);
    video_card_set_draw_circle_func(&drawCircleY);
    video_card_set_draw_rectangle_func(&drawRectangleY);
    video_card_set_clear_screen_func(&clearScreenY);
    bufferTurnedOnY = 0;
    return 0;
}

uint8_t isBufferOnY()
{
    return bufferTurnedOnY;
}

int8_t swapBuffersY()
{
    if(!bufferTurnedOnY) return -1;
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memcpy(VGA_VRAM, MODEY_BUFFER[p], MODEY_WIDTH * MODEY_HEIGHT / 4);
    }
    return 0;
}

int8_t drawPixelY(uint8_t color, uint16_t x, uint16_t y)
{
    if((x>=MODEY_WIDTH) || (y >=MODEY_HEIGHT))
        return -1;
    unsigned char *fb = (unsigned char *) VGA_VRAM;
    unsigned int offset = (y * MODEY_WIDTH + x);
	set_plane(offset % 4);
    fb[offset/4] = color;
    set_plane(0);
    return 0;
}

int8_t drawLineY(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                drawPixelY(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                drawPixelY(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                drawPixelY(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                drawPixelY(color, a * y + b, y);
    }
    return 0;
}

int8_t drawCircleY(uint8_t color, uint16_t x, uint16_t y, uint16_t radius) 
{
    return 0;
}

int8_t drawRectangleY(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t clearScreenY()
{
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memset(VGA_VRAM, 0, 64 * 1024);
    }
    return 0;
}

int8_t drawPixelYBuffered(uint8_t color, uint16_t x, uint16_t y)
{
    if((!bufferTurnedOnY) || (x>=MODEY_WIDTH) || (y >=MODEY_HEIGHT))
        return -1;
    unsigned int offset = (y * MODEY_WIDTH + x);
	MODEY_BUFFER[offset%4][offset/4] = color;
    return 0;
}

int8_t drawLineYBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(!bufferTurnedOnY) return -1;
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                drawPixelYBuffered(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                drawPixelYBuffered(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                drawPixelYBuffered(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                drawPixelYBuffered(color, a * y + b, y);
    }
    return 0;
}
int8_t drawCircleYBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}
int8_t drawRectangleYBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t clearScreenYBuffered()
{
    if(!bufferTurnedOnY) return -1;
    for(uint8_t p = 3; p < 4; p--)
        memset(MODEY_BUFFER[p], 0, 64*1024);
    return 0;
}