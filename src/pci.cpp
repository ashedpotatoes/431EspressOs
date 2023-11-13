#include "pci.h"

namespace PCI {

    void EnumerateFunction(uint64_t deviceAddress, uint64_t function) {
        uint64_t offset = function << 12;

        uint64_t functionAddress = deviceAddress + offset;
        GlobalPTM.MapMemory((void*)functionAddress, (void*)functionAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)functionAddress;


        //if (pciDeviceHeader->DeviceID == 0) return;
        //if (pciDeviceHeader->DeviceID == 0xFFFF) return;

        GlobalRenderer->Print(to_hstring(pciDeviceHeader->VendorID));
        GlobalRenderer->Print(" ");
        GlobalRenderer->Print(to_hstring(pciDeviceHeader->DeviceID));
        GlobalRenderer->Print(" ");
        GlobalRenderer->Print(DeviceClasses[pciDeviceHeader->Class]);
        GlobalRenderer->Next();

    }

    void EnumerateDevice(uint64_t busAddress, uint64_t device) {
        //GlobalRenderer->Print("Device called");
        uint64_t offset = device << 15;

        uint64_t deviceAddress = busAddress + offset;
        GlobalPTM.MapMemory((void*)deviceAddress, (void*)deviceAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)deviceAddress;

        //if (pciDeviceHeader->DeviceID == 0) return;
        //if (pciDeviceHeader->DeviceID == 0xFFFF) return;

        for (uint64_t function = 0; function < 8; function++) {
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64_t baseAddress, uint64_t bus) {
        //GlobalRenderer->Print("Bus called");
        uint64_t offset = bus << 20;

        uint64_t busAddress = baseAddress + offset;
        GlobalPTM.MapMemory((void*)busAddress, (void*)busAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)busAddress;

        if (pciDeviceHeader->DeviceID == 0) return;
        if (pciDeviceHeader->DeviceID == 0xFFFF) return;

        for (uint64_t device = 0; device < 32; device++) {
            EnumerateDevice(busAddress, device);
        }
    }

    void EnumeratePCI(ACPI::MCFGHeader* mcfg) {
        //GlobalRenderer->Print("Enum called");
        int entries = ((mcfg->Header.Length) - sizeof(ACPI::SDTHeader)) / sizeof(ACPI::DeviceConfig);
        GlobalRenderer->Print("Header length: ");
        GlobalRenderer->Print(to_string((int64_t)(mcfg->Header.Length)));
        GlobalRenderer->Print("WAETHeader length: ");
        GlobalRenderer->Print(to_string((int64_t)(sizeof(ACPI::MCFGHeader))));
        GlobalRenderer->Print("Device config: ");
        GlobalRenderer->Print(to_string((int64_t)(sizeof(ACPI::DeviceConfig))));


        for (int t = 0; t < entries; t++) {
            ACPI::DeviceConfig* newDeviceConfig = (ACPI::DeviceConfig*)((uint64_t)mcfg + sizeof(ACPI::MCFGHeader) + (sizeof(ACPI::DeviceConfig) * t));
            for (uint64_t bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++) {
                EnumerateBus(newDeviceConfig->BaseAddress, bus);
                
            }
        }
    }

}