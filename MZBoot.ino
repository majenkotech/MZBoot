#pragma config FMIIEN = ON              // Ethernet RMII/MII Enable (MII Enabled)
#pragma config FETHIO = ON              // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config PGL1WAY = OFF            // Permission Group Lock One Way Configuration (Allow multiple reconfigurations)
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
#pragma config FUSBIDIO = OFF           // USB USBID Selection (Controlled by Port Function)

// DEVCFG2
#pragma config FPLLIDIV = DIV_3         // System PLL Input Divider (1x Divider)
#pragma config FPLLRNG = RANGE_5_10_MHZ // System PLL Input Range (5-10 MHz Input)
#pragma config FPLLICLK = PLL_POSC      // System PLL Input Clock Selection (FRC is input to the System PLL)
#pragma config FPLLMULT = MUL_50        // System PLL Multiplier (PLL Multiply by 50)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (2x Divider)
#pragma config UPLLFSEL = FREQ_24MHZ    // USB PLL Input Frequency Selection (USB PLL input is 24 MHz)

// DEVCFG1
#pragma config FNOSC = SPLL             // Oscillator Selection Bits (System PLL)
#pragma config DMTINTV = WIN_127_128    // DMT Count Window Interval (Window/Interval value is 127/128 counter value)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Enable SOSC)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = EC             // Primary Oscillator Configuration (External clock mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disabled, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WDTSPGM = STOP           // Watchdog Timer Stop During Flash Programming (WDT stops during Flash programming)
#pragma config WINDIS = NORMAL          // Watchdog Timer Window Mode (Watchdog Timer is in non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window size is 25%)
#pragma config DMTCNT = DMT31           // Deadman Timer Count Selection (2^31 (2147483648))
#pragma config FDMTEN = OFF             // Deadman Timer Enable (Deadman Timer is disabled)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
#pragma config TRCEN = ON               // Trace Enable (Trace features in the CPU are enabled)
#pragma config BOOTISA = MIPS32         // Boot ISA Selection (Boot code and Exception code is MIPS32)
#pragma config FECCCON = OFF_UNLOCKED   // Dynamic Flash ECC Configuration (ECC and Dynamic ECC are disabled (ECCCON bits are writable))
#pragma config FSLEEP = OFF             // Flash Sleep Mode (Flash is powered down when the device is in Sleep mode)
#pragma config DBGPER = ALLOW_PG2       // Debug Mode CPU Access Permission (Allow CPU access to Permission Group 2 permission regions)
#pragma config SMCLR = MCLR_NORM        // Soft Master Clear Enable bit (MCLR pin generates a normal system Reset)
#pragma config SOSCGAIN = GAIN_2X       // Secondary Oscillator Gain Control bits (2x gain setting)
#pragma config SOSCBOOST = OFF          // Secondary Oscillator Boost Kick Start Enable bit (Normal start of the oscillator)
#pragma config POSCGAIN = GAIN_1X       // Primary Oscillator Gain Control bits (1x gain setting)
#pragma config POSCBOOST = ON           // Primary Oscillator Boost Kick Start Enable bit (Boost the kick start of the oscillator)
#pragma config EJTAGBEN = NORMAL        // EJTAG Boot (Normal EJTAG functionality)

// DEVCP0
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#include <Flash.h>

USBHS usbDriver;
USBManager USB(usbDriver, 0x04d8, 0x0f5f, "chipKIT", "HID Bootloader");
HID_Raw HID;

volatile uint8_t packet[64];
volatile bool packetValid = false;
volatile uint32_t packetLength = 0;

void (*jumpPoint)() = (void (*)())0x9D001000;

static const uint16_t crc_table[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

uint32_t calculateCRC16(uint8_t *data, uint32_t len) {
    uint32_t i;
    uint16_t crc = 0;

    while(len--) {
        i = (crc >> 12) ^ (*data >> 4);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        i = (crc >> 12) ^ (*data >> 0);
        crc = crc_table[i & 0x0F] ^ (crc << 4);
        data++;
    }

    return (crc & 0xFFFF);
}

void handleIncomingByte(uint8_t b) {
    static uint32_t bpos = 0;
    static bool dle = false;

    if (dle) {
        packet[bpos++] = b;
        dle = false;
    } else {
        switch (b) {
            case 0x01: { // SOH
                bpos = 0;
            }
            break;

            case 0x04: { // EOT
                packetLength = bpos;
                packetValid = true;
            }
            break;

            case 0x10: { // DLE
                dle = true;
            }
            break;

            default: { 
                packet[bpos++] = b;                
            }
        }
    }
}

void outputReport(uint8_t *data, uint32_t len) {
    for (int i = 0; i < len; i++) {
        handleIncomingByte(data[i]);
    }
}

void sendPacket(uint8_t *data, uint32_t len) {
    uint8_t buf[len * 2]; // Enough for every byte to be DLE'd
    uint16_t cs = calculateCRC16(data, len);

    uint32_t bpos = 0;
    
    buf[bpos++] = 0x01;
    for (int i = 0; i < len; i++) {
        uint8_t b = data[i];
        if ((b == 0x01) || (b == 0x04) || (b == 0x10)) {
            buf[bpos++] = 0x10;
        }
        buf[bpos++] = b;
    }
    uint8_t crcl = cs & 0xFF;
    uint8_t crch = cs >> 8;

    if ((crcl == 0x01) || (crcl == 0x04) || (crcl == 0x10)) {
        buf[bpos++] = 0x10;
    }
    buf[bpos++] = crcl;
    
    if ((crch == 0x01) || (crch == 0x04) || (crch == 0x10)) {
        buf[bpos++] = 0x10;
    }
    buf[bpos++] = crch;
    buf[bpos++] = 0x04;

    HID.sendReport(buf, bpos);    
}

void processIHEXLine(uint8_t *data, uint32_t len) {
    static uint16_t offset = 0;
    
    uint8_t dlen = data[0];
    uint16_t addr = data[2] | (data[1] << 8);
    uint8_t type = data[3];
    uint8_t cs = data[len-1];

    switch(type) {
        case 0x04: { // Set address offset
            offset = data[5] | (data[4] << 8);
        }
        break;

        case 0x00: { // Data
            uint32_t fullAddr = (offset << 16) | addr;
            for (int i = 0; i < dlen; i+= 4) {
                uint32_t w = data[4+i] | (data[5+i] << 8) | (data[6+i] << 16) | (data[7+i] << 24);
                Flash.writeWord((void *)fullAddr, w);
                fullAddr += 4;
            }
        }
        break;

        default: {
        }
        break;
    }
}

void executeApp() {
    USB.end();
    disableInterrupts();
    IFS0 = 0;
    IFS1 = 0;
    IFS2 = 0;
    IFS3 = 0;
    IFS4 = 0;
    IFS5 = 0;
    IFS6 = 0;
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    IEC3 = 0;
    IEC4 = 0;
    IEC5 = 0;
    IEC6 = 0;
    for (int i = 0; i < NUM_DIGITAL_PINS; i++) {
        pinMode(i, INPUT);
    }
    jumpPoint();
}

void processAN1388Packet(uint8_t *data, uint32_t len) {
    uint8_t command = data[0];

    switch (command) {
        case 0x01: { // Read bootloader version
            uint8_t resp[3] = {0x01, 1, 5};
            sendPacket(resp, 3);
        }
        break;

        case 0x02: { // Erase flash
            Flash.eraseProgmem();
            uint8_t resp[1] = {0x02};
            sendPacket(resp, 1);
        }
        break;

        case 0x03: { // Program flash
            processIHEXLine(data+1, len-1);
            uint8_t resp[1] = {0x03};
            sendPacket(resp, 1);
        }
        break;

        case 0x04: { // Calculate CRC
            uint32_t start = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
            uint32_t len = data[5] | (data[6] << 8) | (data[7] << 16) | (data[8] << 24);

            uint16_t crc = calculateCRC16((uint8_t *)start, len);
            uint8_t resp[3];
            resp[0] = 0x04;
            resp[1] = crc & 0xFF;
            resp[2] = crc >> 8;
            sendPacket(resp, 3);
        }
        break;
        
        case 0x05: { // Enter application
            executeApp();
        }
        break;

        default: {
        }
        break;
    }
}

void setup() {
   
    pinMode(0, OUTPUT);    
    USB.addDevice(HID);
    HID.onOutputReport(outputReport);
    USB.begin();
}

void loop() {

    static uint32_t ts = millis();

    if (millis() - ts > 5000) {
        executeApp();
    }
    
    if (packetValid) {
        ts = millis();
        if (packetLength >= 3) {
            uint16_t cs = (packet[packetLength - 1] << 8) | packet[packetLength - 2];
            uint16_t newcs = calculateCRC16((uint8_t *)packet, packetLength-2);

            if (cs == newcs) {

                processAN1388Packet((uint8_t *)packet, packetLength - 2);
            }
        }       
        packetValid = false;
    }
}
