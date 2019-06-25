#if defined(_BOARD_PKOB_DA_)
#define _BOARD_VALID_

#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_2X
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_2X
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       ON
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF
/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_3
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config VBATBOREN =  ON
#pragma config DSBOREN =    ON
#pragma config DSWDTPS =    DSPS32
#pragma config DSWDTOSC =   LPRC
#pragma config DSWDTEN =    OFF
#pragma config FDSEN =      ON
#pragma config UPLLFSEL =   FREQ_24MHZ
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config EXTDDRSIZE = DDR_SIZE_128MB
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    OFF
#pragma config PMDL1WAY =   OFF
#pragma config IOL1WAY =    OFF
#define LED PIN_LED1

static inline 
void initDDR() {
    *(volatile uint32_t*)(0xBF8000A0) = 0;
    *(volatile uint32_t*)(0xBF800100) = 0x0A003203;

    while ((*(volatile uint32_t*)(0xBF800100) & 0x80800000) != 0x80800000);

    *(volatile uint32_t*)(0xBF8E9124) = 0x30001000; /* DDRPHYDLLR */
    *(volatile uint32_t*)(0xBF8E9120) = 0x50EE62E3; /* DDRPHYPADCON */
    *(volatile uint32_t*)(0xBF8E9118) = 0x01000053; /* DDRSCLCFG0 */
    *(volatile uint32_t*)(0xBF8E911C) = 0x00000401; /* DDRSCLCFG1 */
    *(volatile uint32_t*)(0xBF8E910C) = 0x00000043; /* DDRSCLLAT */

    *(volatile uint32_t*)(0xBF8E8050) = 0x00000008; /* MEM_WIDTH */

    *(volatile uint32_t*)(0xBF8E8000) = 0x00000000; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8004) = 0x0000001F; /* MIN_LIMIT */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000000; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8008) = 0x000000FF; /* RQST_PERIOD */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000000; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E800C) = 0x00000004; /* MIN_CMD_ACPT */

    *(volatile uint32_t*)(0xBF8E8000) = 0x00000005; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8004) = 0x0000001F; /* MIN_LIMIT */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000008; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8008) = 0x000000FF; /* RQST_PERIOD */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000008; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E800C) = 0x00000010; /* MIN_CMD_ACPT */

    *(volatile uint32_t*)(0xBF8E8000) = 0x0000000A; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8004) = 0x0000001F; /* MIN_LIMIT */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000010; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8008) = 0x000000FF; /* RQST_PERIOD */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000010; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E800C) = 0x00000010; /* MIN_CMD_ACPT */

    *(volatile uint32_t*)(0xBF8E8000) = 0x0000000F; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8004) = 0x00000004; /* MIN_LIMIT */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000018; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8008) = 0x000000FF; /* RQST_PERIOD */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000018; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E800C) = 0x00000004; /* MIN_CMD_ACPT */

    *(volatile uint32_t*)(0xBF8E8000) = 0x00000014; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8004) = 0x00000004; /* MIN_LIMIT */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000020; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E8008) = 0x000000FF; /* RQST_PERIOD */
    *(volatile uint32_t*)(0xBF8E8000) = 0x00000020; /* ARB_AGENT_SEL */
    *(volatile uint32_t*)(0xBF8E800C) = 0x00000004; /* MIN_CMD_ACPT */

    *(volatile uint32_t*)(0xBF8E8014) = 0x20000A0D; /* MEM_CONFIG_0 */
    *(volatile uint32_t*)(0xBF8E8018) = 0x00001FFF; /* MEM_CONFIG_1 */
    *(volatile uint32_t*)(0xBF8E801C) = 0x00000000; /* MEM_CONFIG_2 */
    *(volatile uint32_t*)(0xBF8E8020) = 0x000003FF; /* MEM_CONFIG_3 */
    *(volatile uint32_t*)(0xBF8E8024) = 0x00000007; /* MEM_CONFIG_4 */

    *(volatile uint32_t*)(0xBF8E8028) = 0x07180616; /* REF_CONFIG */

    *(volatile uint32_t*)(0xBF8E802C) = 0x00011080; /* PWR_SAVE_ECC_CONFIG */

    *(volatile uint32_t*)(0xBF8E8030) = 0x04112467; /* DDRDLYCFG0 */
    *(volatile uint32_t*)(0xBF8E8034) = 0x00016202; /* DDRDLYCFG1 */
    *(volatile uint32_t*)(0xBF8E8038) = 0x06229303; /* DDRDLYCFG2 */
    *(volatile uint32_t*)(0xBF8E803C) = 0x00090B08; /* DDRDLYCFG3 */

    *(volatile uint32_t*)(0xBF8E8040) = 0x00000000; /* DDRODTCFG */
    *(volatile uint32_t*)(0xBF8E804C) = 0x00010000; /* DDRODT_EN_CFG */
    *(volatile uint32_t*)(0xBF8E8040) = 0x00121200; /* DDRODTCFG */

    *(volatile uint32_t*)(0xBF8E8044) = 0x73020042; /* DDRXFERCFG */

    *(volatile uint32_t*)(0xBF8E8080) = 0x00FFFFFF; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80C0) = 0x0004F000; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E8084) = 0x00FFF401; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80C4) = 0x00002004; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E8088) = 0x00FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80C8) = 0x00000200; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E808C) = 0x00FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80CC) = 0x00000300; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E8090) = 0x40FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80D0) = 0x00000100; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E8094) = 0x52FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80D4) = 0x0000000B; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E8098) = 0x00FFF401; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80D8) = 0x00002004; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E809C) = 0x00FFF801; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80DC) = 0x00018800; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E80A0) = 0x00FFF801; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80E0) = 0x00018800; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E80A4) = 0x53FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80E4) = 0x0000000A; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E80A8) = 0xC0FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80E8) = 0x00000103; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E80AC) = 0x40FFF001; /* HOST_CMD1 */
    *(volatile uint32_t*)(0xBF8E80EC) = 0x00045100; /* HOST_CMD2 */

    *(volatile uint32_t*)(0xBF8E8048) = 0x0000001B; /* HOST_CMD_ISSUE */
    *(volatile uint32_t*)(0xBF8E8010) = 0x00000001; /* MEM_START */

    while ((*(volatile uint32_t*)(0xBF8E8048) & 0x00000010) == 0x00000010);

    *(volatile uint32_t*)(0xBF8E8010) = 0x00000003; /* MEM_START */

    *(volatile uint32_t*)(0xBF8E9100) = 0x14000000; /* MEM_START */
    while ((*(volatile uint32_t*)(0xBF8E9100) & 0x00000003) != 0x00000003);
}

#define INIT_FUNC initDDR();

#define MODE_SERIAL
#define BAUD 115200
#define SERIAL Serial
//#define USBDEV USBHS

#endif
