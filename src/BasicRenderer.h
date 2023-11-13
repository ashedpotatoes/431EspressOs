#pragma once
#include "math.h"
#include "Framebuffer.h"
#include "SimpleFont.h"
#include <stdint.h>

class BasicRenderer {
public:
    BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    Framebuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    uint32_t MouseCursorBuffer[16 * 16];
    uint32_t MouseCursorBufferAfter[16 * 16];
    unsigned int ClearColour; 
    unsigned int Colour;
    void Print(const char* str);
    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
    void PutChar(char chr);
    void Clear();
    void Next();
    void ClearChar(); 
    void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour);
    void putPix(uint32_t x, uint32_t y, uint32_t colour);
    uint32_t getPix(uint32_t x, uint32_t y); 
    void ClearMouseCursor(uint8_t* mouseCursor,Point position);
    bool MouseDrawn;

};

extern BasicRenderer * GlobalRenderer;
