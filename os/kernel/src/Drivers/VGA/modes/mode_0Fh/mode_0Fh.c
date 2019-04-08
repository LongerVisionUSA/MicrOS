#include "mode_0FH.h"
#include "Drivers/VGA/modes/registerFunctions.h"
#include "Memory/Manager/Heap/heap.h"
#include "Drivers/DAL/VideoCard/videocard.h"

//REGISTER VALUES
unsigned char g_640x350x3[] =
	{
	/* MISC */
		0xA2, 
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x06, 
	/* CRTC */
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05,
		0xFF, 
	/* AC */
		0x00, 0x01, 0x00, 0x03, 0x18, 0x05, 0x00, 0x07,
		0x00, 0x09, 0x00, 0x0B, 0x00, 0x0D, 0x00, 0x0F,
		0x0B, 0x01, 0x05, 0x13, 0x00, 
	};

//PALETTE
unsigned char palette0FH[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
    0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
    0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
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

unsigned char *MODE0FH_BUFFER[] = {NULL, NULL};
unsigned char bufferTurnedOn0FH = 0;

int8_t setMode0FH()
{
    writeRegisters(g_640x350x3);
    set_vga_palette(palette0FH);
    clearScreen0FH();
    setTurnOnBufferFunc(&turnOnBuffer0FH);
    setTurnOffBufferFunc(&turnOffBuffer0FH);
    setIsBufferOnFunc(&isBufferOn0FH);
    setSwapBuffersFunc(&swapBuffers0FH);
    setDrawPixelFunc(&drawPixel0FH);
    setDrawLineFunc(&drawLine0FH);
    setDrawCircleFunc(&drawCircle0FH);
    setDrawRectangleFunc(&drawRectangle0FH);
    setClearScreenFunc(&clearScreen0FH);
    return 0x0F;
}

int8_t turnOnBuffer0FH()
{
    if(bufferTurnedOn0FH) return -1;
    for(int i = 1; i >= 0; i--)
    {
        MODE0FH_BUFFER[i] = heap_kernel_alloc(MODE0FH_HEIGHT * MODE0FH_WIDTH / 8, 0);
        if(MODE0FH_BUFFER[i] == NULL)
        {
            for(int j = 1; j >= i; j--)
            {
                heap_kernel_dealloc(MODE0FH_BUFFER[j]);
                MODE0FH_BUFFER[j] = NULL;
            }
            return -1;
        }
    }
    setDrawPixelFunc(&drawPixel0FHBuffered);
    setDrawLineFunc(&drawLine0FHBuffered);
    setDrawCircleFunc(&drawCircle0FHBuffered);
    setDrawRectangleFunc(&drawRectangle0FHBuffered);
    setClearScreenFunc(&clearScreen0FHBuffered);
    bufferTurnedOn0FH = 1;
    return 0;
}

int8_t turnOffBuffer0FH()
{
    if(!bufferTurnedOn0FH) return -1;
    for(int i = 1; i >= 0; i--)
    {
        heap_kernel_dealloc(MODE0FH_BUFFER[i]);
        MODE0FH_BUFFER[i] = NULL;
    }
    setDrawPixelFunc(&drawPixel0FH);
    setDrawLineFunc(&drawLine0FH);
    setDrawCircleFunc(&drawCircle0FH);
    setDrawRectangleFunc(&drawRectangle0FH);
    setClearScreenFunc(&clearScreen0FH);
    bufferTurnedOn0FH = 0;
    return 0;
}

uint8_t isBufferOn0FH()
{
    return bufferTurnedOn0FH;
}

int8_t swapBuffers0FH()
{
    if(!bufferTurnedOn0FH) return -1;
    for(uint8_t p = 1; p < 2; p--)
    {
        set_plane(p*2);
        memcpy(VGA_VRAM, MODE0FH_BUFFER[p], MODE0FH_WIDTH * MODE0FH_HEIGHT / 8);
    }
    //memcpy(VGA_VRAM, MODE13H_BUFFER, MODE13H_HEIGHT * MODE13H_WIDTH);
    return 0;
}

int8_t drawPixel0FH(uint8_t color, uint16_t x, uint16_t y)
{
    if((x>=MODE0FH_WIDTH) || (y >=MODE0FH_HEIGHT))
        return -1;
    unsigned char *fb = (unsigned char *) VGA_VRAM;
    unsigned int offset = (y * MODE0FH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(char p = 1; p >= 0; p--)
	{
		set_plane(p*2);
		bit_write(fb[offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
	}
    return 0;
}

int8_t drawLine0FH(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
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
                drawPixel0FH(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                drawPixel0FH(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                drawPixel0FH(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                drawPixel0FH(color, a * y + b, y);
    }
    return 0;
}

int8_t drawCircle0FH(uint8_t color, uint16_t x, uint16_t y, uint16_t radius) 
{
    return 0;
}

int8_t drawRectangle0FH(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t clearScreen0FH()
{
    for(uint8_t p = 1; p < 2; p--)
    {
        set_plane(p * 2);
        memset(VGA_VRAM, 0, 64 * 1024);
    }
    return 0;
}

int8_t drawPixel0FHBuffered(uint8_t color, uint16_t x, uint16_t y)
{
    if((!bufferTurnedOn0FH) || (x>=MODE0FH_WIDTH) || (y >=MODE0FH_HEIGHT))
        return -1;
    unsigned int offset = (y * MODE0FH_WIDTH + x)/8;
	unsigned bit_no = x % 8;
	for(uint8_t p = 1; p < 2; p--)
		bit_write(MODE0FH_BUFFER[p][offset], 1<<(7-bit_no), (bit_get(color, 1 << p)));
    return 0;
}

int8_t drawLine0FHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    if(!bufferTurnedOn0FH) return -1;
    if(ax == bx) return -1;
    int32_t dx = (int32_t)bx - ax;
    int32_t dy = (int32_t)by - ay;
    if(_abs(dx) >= _abs(dy))
    {
        float a = dy/(float)(dx);
        float b = ay - a * ax;
        if(ax > bx)
            for(int x = bx; x <= ax; ++x)
                drawPixel0FHBuffered(color, x, a * x + b);
        else
            for(int x = ax; x <= bx; ++x)
                drawPixel0FHBuffered(color, x, a * x + b);
    }
    else
    {
        float a = dx/(float)(dy);
        float b = ax - a * ay;
        if(ay > by)
            for(int y = by; y <= ay; ++ y)
                drawPixel0FHBuffered(color, a * y + b, y);
        else
            for(int y = ay; y <= by; ++ y)
                drawPixel0FHBuffered(color, a * y + b, y);
    }
    return 0;
}
int8_t drawCircle0FHBuffered(uint8_t color, uint16_t x, uint16_t y, uint16_t radius)
{
    return 0;
}
int8_t drawRectangle0FHBuffered(uint8_t color, uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by)
{
    return 0;
}
int8_t clearScreen0FHBuffered()
{
    if(!bufferTurnedOn0FH) return -1;
    for(uint8_t p = 1; p < 2; p--)
        memset(MODE0FH_BUFFER[p], 0, 64*1024);
    return 0;
}