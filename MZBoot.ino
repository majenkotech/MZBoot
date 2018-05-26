/*
 * Copyright (c) 2018, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Flash.h>
#include <SoftPWMServo.h>

#define ENABLE_DEBUG 0

#include "configs.h"
#include "DeviceSetup.h"

volatile uint8_t packet[64];
volatile bool packetValid = false;
volatile uint32_t packetLength = 0;

void (*jumpPoint)() = (void (*)())(BOOT_JUMP);

static const uint16_t crc_table[16] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

#define PAGES ((2*1024*1024)/BYTE_PAGE_SIZE)
#define PAGENO(X) (((X) & 0xFFFFFF) / BYTE_PAGE_SIZE)

uint8_t erasedPages[PAGES] = {0};

bool isErased(uint32_t addr) {
    uint32_t off = PAGENO(addr);
    if (off >= PAGES) return 1;
    return erasedPages[off];
}

void setErased(uint32_t addr) {
    uint32_t off = PAGENO(addr);
    if (off < PAGES) {
        Flash.erasePage((void *)(addr));
        erasedPages[off] = 1;    
    }
}

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
        bpos = bpos % 64; 
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
                bpos = bpos % 64; 
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

    #if defined(MODE_HID)
    HID.sendReport(buf, bpos);    
    #elif defined(MODE_CDCACM)
    uSerial.write(buf, bpos);
    #elif defined(MODE_SERIAL)
    SERIAL.write(buf, bpos);
    #endif
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
                if ((fullAddr & 0x1FFFFFFF) < 0x1FC00000) {
                    if (!isErased(fullAddr)) {
                        
                        setErased(fullAddr);
                    }
                    Flash.writeWord((void *)fullAddr, w);
                }
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

    #if defined(MODE_HID) || defined(MODE_CDCACM)
    USB.end();
    #elif defined(MODE_SERIAL)
    SERIAL.end();
    #endif


    for (int i = 0; i < NUM_DIGITAL_PINS; i++) {
        pinMode(i, INPUT);
    }

    disableInterrupts();
    #ifdef IFS0
    IFS0 = 0;
    IEC0 = 0;
    #endif

    #ifdef IFS1
    IFS1 = 0;
    IEC1 = 0;
    #endif

    #ifdef IFS2
    IFS2 = 0;
    IEC2 = 0;
    #endif

    #ifdef IFS3
    IFS3 = 0;
    IEC3 = 0;
    #endif

    #ifdef IFS4
    IFS4 = 0;
    IEC4 = 0;
    #endif

    #ifdef IFS5
    IFS5 = 0;
    IEC5 = 0;
    #endif

    #ifdef IFS6
    IFS6 = 0;
    IEC6 = 0;
    #endif
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
            // We don't actually do anything here. This is to protect
            // the split flash area of smaller chips.  We'll say that
            // we did it though, just to keep pic32prog happy.
            // Flash.eraseProgmem();
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

        case 0x06: { // Get device ID
            uint8_t resp[5];
            resp[0] = 0x06;
            resp[1] = DEVID;
            resp[2] = DEVID >> 8;
            resp[3] = DEVID >> 16;
            resp[4] = DEVID >> 24;
            sendPacket(resp, 5);
        }
        break;

        default: {
        }
        break;
    }
}

void setup() {
#ifdef INIT_FUNC
    INIT_FUNC
#endif

    #ifdef BUTTON
    pinMode(BUTTON, INPUT_PULLUP);
    delay(1);
    if (digitalRead(BUTTON) == HIGH) {
        executeApp();
    }
    #endif

    #ifdef LED
    pinMode(LED, OUTPUT);
    #endif

#if defined(MODE_HID)
    USB.addDevice(HID);
    HID.onOutputReport(outputReport);
#if (ENABLE_DEBUG == 1)
    USB.addDevice(debugSerial);
#endif
    USB.begin();



#elif defined(MODE_CDCACM)
    USB.addDevice(uSerial);
    HID.onOutputReport(outputReport);
#if (ENABLE_DEBUG == 1)
    USB.addDevice(debugSerial);
#endif
    USB.begin();


#elif defined(MODE_SERIAL)
    SERIAL.begin(BAUD);

    
#endif

}

void loop() {
    #ifdef LED
    static int bright = 0;
    static int fade = 1;
    static uint32_t led_ts = millis();
    if (millis() - led_ts > 0) {
        led_ts = millis();
        bright += fade;
        if (bright >= 255) {
            bright = 255;
            fade = -fade;
        }

        if (bright <= 0) {
            bright = 0;
            fade = -fade;
        }

        SoftPWMServoPWMWrite(LED, bright);

        
    }
    #endif

    static uint32_t ts = millis();


    #if defined(MODE_CDCACM)
    while (uSerial.available() && !packetValid) {
        handleIncomingByte(uSerial.read());
    }
    #elif defined(MODE_SERIAL)
    while (SERIAL.available() && !packetValid) {
        handleIncomingByte(SERIAL.read());
    }
    #endif

    if (millis() - ts > (BOOT_TIMEOUT_SECONDS * 1000)) {
        executeApp();
    }
    
    if (packetValid) {
        DEBUG("Packet valid\r\n");
        ts = millis();
        if (packetLength >= 3) {
            uint16_t cs = (packet[packetLength - 1] << 8) | packet[packetLength - 2];
            uint16_t newcs = calculateCRC16((uint8_t *)packet, packetLength-2);
            DEBUG("Checksum %02x => %02x\r\n", cs, newcs);

            if (cs == newcs) {
                processAN1388Packet((uint8_t *)packet, packetLength - 2);
            }
        }       
        packetValid = false;
    }
}

