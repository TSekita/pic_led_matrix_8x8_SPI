
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
// #include <stdint.h>

#define _XTAL_FREQ 1000000 // INTERNAL OSCILLATOR 1MHz
#define LOAD_TRIS TRISCbits.TRISC2
#define LOAD_LAT LATCbits.LATC2

void SPI1_Init(void) {
    
    RC3PPS = 0x14;       // RC3  SCK1 output
    RC4PPS = 0x15;       // RC4  SDO1 output
//    SSP1CLKPPS = 0x13;   // SCK1 input  RC3
    //SSP1DATPPS = 0x15;   // SDO1 input  RC5
    
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    //TRISCbits.TRISC5 = 1;
    
    SSP1STAT = 0x40;
    SSP1CON1 = 0x20;
    
    SSP1CON1bits.SSPEN = 1;
}

void SPI1_Write(uint8_t data) {
    SSP1BUF = data;
    while (!SSP1STATbits.BF);
    // (void)SSP1BUF;
}

void MAX7219_send(uint8_t address, uint8_t data) {
    LOAD_LAT = 0;
    __delay_us(1);
    SPI1_Write(address);
    SPI1_Write(data);
    LOAD_LAT = 1;
    __delay_us(10);
}

void main(void) {
    SPI1_Init();
    
    LOAD_TRIS = 0;
    LOAD_LAT = 1;
    
    __delay_ms(200);
    
    MAX7219_send(0x0F, 0x00);
    MAX7219_send(0x09, 0x00);
    MAX7219_send(0x0A, 0x05);
    MAX7219_send(0x0B, 0x07);
    MAX7219_send(0x0C, 0x01);
    
    
    for (int i = 1; i <= 8; i++){
        MAX7219_send(i, 0x00);
    }
    MAX7219_send(0x01, 0x0F);
//    while (1);
}

