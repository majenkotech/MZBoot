
#if defined(MODE_HID)
USBDEV usbDriver;
USBManager USB(usbDriver, 0x04d8, 0x0f5f, "chipKIT", "HID Bootloader");
HID_Raw HID;

#elif defined(MODE_CDCACM)
USBDEV usbDriver;
USBManager USB(usbDriver, 0x04d8, 0x0f5f, "chipKIT", "CDCACM Bootloader");
CDCACM uSerial;

#elif defined(MODE_SERIAL)
#if !defined(SERIAL)
#error MODE_SERIAL defined but no SERIAL defined.
#endif
#if !defined(BAUD)
#error MODE_SERIAL defined but no BAUD defined.
#endif

#else
#error No MODE_x specified
#endif

#if (ENABLE_DEBUG == 1)
CDCACM debugSerial;
#define DEBUG(...) debugSerial.printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#ifndef BOOT_TIMEOUT_SECONDS
#define BOOT_TIMEOUT_SECONDS 15
#endif

#ifndef BOOT_JUMP
#define BOOT_JUMP 0x9D001000
#endif
