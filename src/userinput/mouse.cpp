#include "mouse.h"

uint8_t MousePointer[] = {
    // customizable :]]]]]]] -- make steam or a little cup 
    0b11111111, 0b11100000,
    0b11111111, 0b10000000,
    0b11111110, 0b00000000,
    0b11111100, 0b00000000,
    0b11111000, 0b00000000,
    0b11110000, 0b00000000,
    0b11100000, 0b00000000,
    0b11000000, 0b00000000,
    0b11000000, 0b00000000,
    0b10000000, 0b00000000,
    0b10000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
};
void MouseWait() {
    uint64_t timeout = 10000000;
    while (timeout--) {
        if ((inb(0x64) & 0b10) == 0) {
            return;
        }
    }
}

void MouseWaitInput() {
    uint64_t timeout = 1000000;
    while (timeout--) {
        if (inb(0x64) & 0b1) {
            return;
        }
    }
}

void MouseWrite(uint8_t value) {
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead() {
    MouseWaitInput();
    return inb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
Point MousePositionOld;
void HandlePS2Mouse(uint8_t data) {

    switch (MouseCycle) {
    case 0:
        if (MousePacketReady) break;
        if ((data & 0b00001000) == 0) break;
        MousePacket[0] = data;
        MouseCycle++;
        break;
    case 1:
        if (MousePacketReady) break;
        MousePacket[1] = data;
        MouseCycle++;
        break;
    case 2:
        if (MousePacketReady) break;
        MousePacket[2] = data;
        MousePacketReady = true;
        MouseCycle = 0;
        break;
    }
}

void ProcessMousePacket() {
    if (!MousePacketReady) return;

    bool xNegative, yNegative, xOverflow, yOverflow;

    if (MousePacket[0] & PS2XSign) {
        xNegative = true;
    }
    else xNegative = false;

    if (MousePacket[0] & PS2YSign) {
        yNegative = true;
    }
    else yNegative = false;

    if (MousePacket[0] & PS2XOverflow) {
        xOverflow = true;
    }
    else xOverflow = false;

    if (MousePacket[0] & PS2YOverflow) {
        yOverflow = true;
    }
    else yOverflow = false;

    if (!xNegative) {
        MousePosition.x += MousePacket[1];
        if (xOverflow) {
            MousePosition.x += 255;
        }
    }
    else
    {
        MousePacket[1] = 256 - MousePacket[1];
        MousePosition.x -= MousePacket[1];
        if (xOverflow) {
            MousePosition.x -= 255;
        }
    }

    if (!yNegative) {
        MousePosition.y -= MousePacket[2];
        if (yOverflow) {
            MousePosition.y -= 255;
        }
    }
    else
    {
        MousePacket[2] = 256 - MousePacket[2];
        MousePosition.y += MousePacket[2];
        if (yOverflow) {
            MousePosition.y += 255;
        }
    }

    if (MousePosition.x < 0) MousePosition.x = 0;
    if (MousePosition.x > GlobalRenderer->TargetFramebuffer->Width - 1) MousePosition.x = GlobalRenderer->TargetFramebuffer->Width - 1;

    if (MousePosition.y < 0) MousePosition.y = 0;
    if (MousePosition.y > GlobalRenderer->TargetFramebuffer->Height - 1) MousePosition.y = GlobalRenderer->TargetFramebuffer->Height - 1;

    GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
    GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0xffffffff);

    if (MousePacket[0] & PS2Leftbutton) {
        GlobalRenderer->PutChar('a', MousePosition.x, MousePosition.y);
    }

    if (MousePacket[0] & PS2Middlebutton) {

    }

    if (MousePacket[0] & PS2Rightbutton) {
        uint32_t colour = GlobalRenderer->Colour;
        GlobalRenderer->Colour = 0x0000ff00;
        GlobalRenderer->PutChar('a', MousePosition.x, MousePosition.y);
        GlobalRenderer->Colour = colour;
    }
    MousePacketReady = false;
    MousePositionOld = MousePosition; 
}


void InitPS2Mouse() {
    /*outb(0xD4, 0x64);                    // tell the controller to address the mouse
    outb(0x64, 0xA8);
    outb(0xF3, 0x60);                    // write the mouse command code to the controller's data port

    MouseWait();
    outb(0x64, 0x20);

    MouseWait();
    uint8_t ack = inb(0x60);                     // read back acknowledge. This should be 0xFA
    outb(0xD4, 0x64);                    // tell the controller to address the mouse
    outb(100, 0x60);                     // write the parameter to the controller's data port
    MouseWait();
    ack = inb(0x60);                     // read back acknowledge. This should be 0xFA

    MouseWrite(0xf6);
    MouseRead();

    MouseWrite(0xf4);
    MouseRead();*/

    outb(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    outb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();

}