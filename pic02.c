
// PIC16F18857 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT1  // Power-up default value for COSC bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (FSCM timer disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = OFF        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = not_available// Scanner Enable bit (Scanner module is not available for use)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
//#include <stdio.h>

#define _XTAL_FREQ 1000000 // INTERNAL OSCILLATOR 1MHz
#define MAX7219_LOAD_TRIS TRISCbits.TRISC2
#define MAX7219_LOAD_LAT LATCbits.LATC2
//#define MFRC522_NSS_TRIS TRISCbits.TRISC5
//#define MFRC522_NSS_LAT LATCbits.LATC5

void SPI1_Init(void) {
    
//    SSP2DATPPS = 0x0A;   // RB2  SDI2/SDA2
    RC3PPS = 0x14;       // RC3  SCK1/SCL1
    RC4PPS = 0x15;       // RC4  SDO1/SDA1

//    TRISBbits.TRISB2 = 1; // input
    TRISCbits.TRISC3 = 0; // output
    TRISCbits.TRISC4 = 0; // output
//    TRISCbits.TRISC5 = 0; // output

    SSP1STAT = 0x40;
    SSP1CON1 = 0x00;
    
    SSP1CON1bits.SSPEN = 1;
}

void SPI1_Write(uint8_t data) {
    SSP1BUF = data;
    while (!SSP1STATbits.BF);
    (void) SSP1BUF;
}

//uint8_t SPI1_Write(uint8_t data) {
//    SSP1BUF = data;
//    while (!SSP1STATbits.BF);
//    return SSP1BUF;
//}

void MAX7219_send(uint8_t address, uint8_t data) {
    MAX7219_LOAD_LAT = 0;
    __delay_us(1);
    SPI1_Write(address);
    SPI1_Write(data);
    MAX7219_LOAD_LAT = 1;
    __delay_us(10);
}

//uint8_t MFRC522_read(uint8_t address) {
//    MFRC522_NSS_LAT = 0;
//    __delay_us(1);
//    SPI1_Write((address << 1) & 0x7E | 0x80);
//    uint8_t value = SPI1_Write(0x00);
//    MFRC522_NSS_LAT = 1;
//    __delay_us(10);
//    return value;
//}

void main(void) {
    SPI1_Init();
    
    MAX7219_LOAD_TRIS = 0; //output
    MAX7219_LOAD_LAT = 1;  // high
//    MFRC522_NSS_TRIS = 0;  //output
//    MFRC522_NSS_LAT = 1;   // high
    
    __delay_ms(100);
    
    MAX7219_send(0x0F, 0x00); // all led off
    MAX7219_send(0x09, 0x00);
    MAX7219_send(0x0A, 0x00); // brightness intensity
    MAX7219_send(0x0B, 0x07);
    MAX7219_send(0x0C, 0x01);
    
    
    for (int i = 1; i <= 8; i++){
        MAX7219_send(i, 0x00);
    }
    
//    uint8_t version = MFRC522_read(0x37);
//    if (version == 0x91) {
        while (1) {
        for (int i = 1; i <= 8; i++){
            for (int j = 0; j < 8; j++) {
                MAX7219_send(i, 0x01 << j);
                __delay_ms(500);
            }
            MAX7219_send(i, 0x00);
        }
//    }
    }
//    else {
//        while (1) {
//        for (int i = 8; i >= 1; i++){
//            for (int j = 0; j < 8; j++) {
//                MAX7219_send(i, 0x80 >> j);
//                __delay_ms(500);
//            }
//            MAX7219_send(i, 0x00);
//        }
//    }
//    }
}

