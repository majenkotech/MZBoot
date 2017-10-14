
#include <Flash.h>

USBHS usbDriver;
USBManager USB(usbDriver, 0x04d8, 0x0f5f, "chipKIT", "HID Bootloader");
HID_Raw HID;
//CDCACM uSerial;

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
//                uSerial.printf("Write word at %08x: %08x\r\n", fullAddr, w);
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
    executeSoftReset(0);
    USB.end();
    disableInterrupts();
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

//            uSerial.printf("Calc checksum of %08x for %d bytes\r\n", start, len);

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

    // If we did a software reset then immediately run the application.
    if (RCONbits.SWR) {
        disableInterrupts();
        jumpPoint();
    }
    
    pinMode(0, OUTPUT);    
    USB.addDevice(HID);
//    USB.addDevice(uSerial);
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
//            uSerial.printf("Got packet of length %d\r\n", packetLength);
//            for (int i = 0; i < packetLength; i++) {
//                uSerial.printf("  %02x", packet[i]);
//            }
//            uSerial.println();    
            uint16_t cs = (packet[packetLength - 1] << 8) | packet[packetLength - 2];
            uint16_t newcs = calculateCRC16((uint8_t *)packet, packetLength-2);

            if (cs == newcs) {
//                uSerial.printf("  Checksum: %04x == %04x\r\n", cs, newcs);            

                processAN1388Packet((uint8_t *)packet, packetLength - 2);
            } else {
//                uSerial.printf("  Checksum Bad\r\n");
            }
        }       
        packetValid = false;
    }
}
