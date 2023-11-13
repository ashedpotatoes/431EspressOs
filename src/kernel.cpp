#include "kernelUtil.h"
#include "memory/heap.h"

extern "C" void _start(BootInfo * bootInfo) {

    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;

    GlobalRenderer->Next();
    GlobalRenderer->Print("Welcome to EspressOs!");
    GlobalRenderer->Next();

    while (true) {
        ProcessMousePacket();
    }


    while (true);
}