#include "mode_0EH.h"
#include "Drivers/VGA/modes/registerFunctions.h"
#include "Memory/Manager/Heap/heap.h"
#include "Drivers/DAL/VideoCard/videocard.h"

//REGISTER VALUES
unsigned char g_640x200x16[] =
	{
	/* MISC */
		0x63, 
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06, 
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x8E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3,
		0xFF, 
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
		0xFF, 
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x01, 0x00, 0x0F, 0x00, 0x00, 
	};

//PALETTE
unsigned char palette0EH[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 
    0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 
    0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 
    0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 
    0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 
    0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 
    0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 
    0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 
    0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 
    0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 
    0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//DOUBLE BUFFER POINTER;

unsigned char *MODE0EH_BUFFER[] = {NULL, NULL, NULL, NULL};
unsigned char bufferTurnedOn0EH = 0;

int8_t setMode0EH()
{
    writeRegisters(g_640x200x16);
    set_vga_palette(palette0EH);
    clearScreen0EH();
    setTurnOnBufferFunc(&turnOnBuffer0EH);
    setTurnOffBufferFunc(&turnOffBuffer0EH);
    setIsBufferOnFunc(&isBufferOn0EH);
    setSwapBuffersFunc(&swapBuffers0EH);
    setDrawPixelFunc(&drawPixel0EH);
    setDrawLineFunc(&drawLine0EH);
    setDrawCircleFunc(&drawCircle0EH);
    setDrawRectangleFunc(&drawRectangle0EH);
    setClearScreenFunc(&clearScreen0EH);
    return 0x0E;
}

int8_t turnOnBuffer0EH()
{
    if(bufferTurnedOn0EH) return -1;
    for(int i = 3; i >= 0; i--)
    {
        MODE0EH_BUFFER[i] = heap_kernel_alloc(MODE0EH_HEIGHT * MODE0EH_WIDTH / 8, 0);
        if(MODE0EH_BUFFER[i] == NULL)
        {
            for(int j = 3; j >= i; j--)
            {
                heap_kernel_dealloc(MODE0EH_BUFFER[j]);
                MODE0EH_BUFFER[j] = NULL;
            }
            return -1;
        }
    }
    setDrawPixelFunc(&drawPixel0EHBuffered);
    setDrawLineFunc(&drawLine0EHBuffered);
    setDrawCircleFunc(&drawCircle0EHBuffered);
    setDrawRectangleFunc(&drawRectangle0EHBuffered);
    setClearScreenFunc(&clearScreen0EHBuffered);
    bufferTurnedOn0EH = 1;
    return 0;
}

int8_t turnOffBuffer0EH()
{
    if(!bufferTurnedOn0EH) return -1;
    for(int i = 3; i >= 0; i--)
    {
        heap_kernel_dealloc(MODE0EH_BUFFER[i]);
        MODE0EH_BUFFER[i] = NULL;
    }
    setDrawPixelFunc(&drawPixel0EH);
    setDrawLineFunc(&drawLine0EH);
    setDrawCircleFunc(&drawCircle0EH);
    setDrawRectangleFunc(&drawRectangle0EH);
    setClearScreenFunc(&clearScreen0EH);
    bufferTurnedOn0EH = 0;
    return 0;
}

uint8_t isBufferOn0EH()
{
    return bufferTurnedOn0EH;
}

int8_t swapBuffers0EH()
{
    if(!bufferTurnedOn0EH) return -1;
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memcpy(VGA_VRAM, MODE0EH_BUFFER[p], MODE0EH_WIDTH * MODE0EH_HEIGHT / 8);
    }
    return 0;
}

int8_t drawPixel0EH(uint8_t color, uint16_t x, uint16_t y)
{
    if((x>=MODE0EH_WIDTH) || (y >=MODE0EH_HEIGHT))
        return -1;
    unsigned char *fb = (unsigned char *) VGA_VRAM;
    unsigned int offset = (y * MODE0EH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(char p = 3; p >= 0; p--)
	{
		set_plane(p);
		bit_write(fb[offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
	}
    return 0;
}

int8_t drawLine0EH(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
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
                drawPixel0EH(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                drawPixel0EH(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                drawPixel0EH(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                drawPixel0EH(color, a * y + b, y);
    }
    return 0;
}

int8_t drawCircle0EH(uint8_t color, uint16_t x, uint16_t y, uint16_t radius) 
{
    return 0;
}

int8_t drawRectangle0EH(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t clearScreen0EH()
{
    for(uint8_t p = 3; p < 4; p--)
    {
        set_plane(p);
        memset(VGA_VRAM, 0, 64 * 1024);
    }
    return 0;
}

int8_t drawPixel0EHBuffered(uint8_t color, uint16_t x, uint16_t y)
{
    if((!bufferTurnedOn0EH) || (x>=MODE0EH_WIDTH) || (y >=MODE0EH_HEIGHT))
        return -1;
    unsigned int offset = (y * MODE0EH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(uint8_t p = 3; p < 4; p--)
		bit_write(MODE0EH_BUFFER[p][offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
    return 0;
}

int8_t drawLine0EHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(!bufferTurnedOn0EH) return -1;
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                drawPixel0EHBuffered(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                drawPixel0EHBuffered(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                drawPixel0EHBuffered(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                drawPixel0EHBuffered(color, a * y + b, y);
    }
    return 0;
}
int8_t drawCircle0EHBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}
int8_t drawRectangle0EHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t clearScreen0EHBuffered()
{
    if(!bufferTurnedOn0EH) return -1;
    for(uint8_t p = 3; p < 4; p--)
        memset(MODE0EH_BUFFER[p], 0, 64*1024);
    return 0;
}