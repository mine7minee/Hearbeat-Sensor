#include <xc.h>
#include <stdio.h>

// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000

// LCD pin definitions
#define LCD_RS RD2
#define LCD_EN RD3
#define LCD_D4 RD4
#define LCD_D5 RD5
#define LCD_D6 RD6
#define LCD_D7 RD7

// Heartbeat sensor pin definition
#define HEARTBEAT_PIN 0 // Assuming we're using AN0 (RA0) for the analog input

// Function prototypes
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_Init();
void LCD_String(const char *str);
void LCD_SetCursor(unsigned char row, unsigned char col);
void ADC_Init();
unsigned int ADC_Read(unsigned char channel);

// LCD functions
void LCD_Command(unsigned char cmd) {
    LCD_RS = 0;
    LCD_D7 = (cmd & 0x80) ? 1 : 0;
    LCD_D6 = (cmd & 0x40) ? 1 : 0;
    LCD_D5 = (cmd & 0x20) ? 1 : 0;
    LCD_D4 = (cmd & 0x10) ? 1 : 0;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
    LCD_D7 = (cmd & 0x08) ? 1 : 0;
    LCD_D6 = (cmd & 0x04) ? 1 : 0;
    LCD_D5 = (cmd & 0x02) ? 1 : 0;
    LCD_D4 = (cmd & 0x01) ? 1 : 0;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
}

void LCD_Char(unsigned char data) {
    LCD_RS = 1;
    LCD_D7 = (data & 0x80) ? 1 : 0;
    LCD_D6 = (data & 0x40) ? 1 : 0;
    LCD_D5 = (data & 0x20) ? 1 : 0;
    LCD_D4 = (data & 0x10) ? 1 : 0;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
    LCD_D7 = (data & 0x08) ? 1 : 0;
    LCD_D6 = (data & 0x04) ? 1 : 0;
    LCD_D5 = (data & 0x02) ? 1 : 0;
    LCD_D4 = (data & 0x01) ? 1 : 0;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
}

void LCD_Init() {
    TRISD = 0x00;  // Set PORTD as output
    __delay_ms(15);
    
    LCD_Command(0x02);  // Return home
    __delay_ms(2);
    LCD_Command(0x28);  // 4-bit mode, 2 lines, 5x8 font
    __delay_ms(1);
    LCD_Command(0x0C);  // Display on, cursor off
    __delay_ms(1);
    LCD_Command(0x06);  // Increment cursor
    __delay_ms(1);
    LCD_Command(0x01);  // Clear display
    __delay_ms(2);
}

void LCD_String(const char *str) {
    while (*str)
        LCD_Char(*str++);
}

void LCD_SetCursor(unsigned char row, unsigned char col) {
    unsigned char address;
    if (row == 0)
        address = 0x80 + col;
    else
        address = 0xC0 + col;
    LCD_Command(address);
}

// ADC functions
void ADC_Init() {
    ADCON0 = 0x41;  // ADC ON, Fosc/16, Channel 0
    ADCON1 = 0x80;  // Right justified, All pins as analog
    ADON = 1;       // Turn on ADC module
}

unsigned int ADC_Read(unsigned char channel) {
    ADCON0 &= 0xC5;  // Clear channel selection bits
    ADCON0 |= channel << 3;  // Set channel selection bits
    __delay_us(20);  // Acquisition time delay
    GO_nDONE = 1;  // Start conversion
    while(GO_nDONE);  // Wait for conversion to complete
    return ((ADRESH << 8) + ADRESL);  // Return result
}

void main() {
    LCD_Init();
    ADC_Init();
    __delay_ms(100);
    
    unsigned int heartbeat_value;
    char heartbeat_str[16];
    
    while(1) {
        heartbeat_value = ADC_Read(HEARTBEAT_PIN);
        
        
        // Convert ADC value to BPM (this is a simplified calculation)
        // You may need to adjust this based on your sensor's characteristics
        unsigned int bpm = heartbeat_value / 10;
        
        LCD_Command(0x01);  // Clear display
        __delay_ms(2);
        
        LCD_SetCursor(0, 0);
        LCD_String("Heartbeat:");
        
        LCD_SetCursor(1, 0);
        sprintf(heartbeat_str, "BPM: %u", bpm);
        LCD_String(heartbeat_str);
        
        __delay_ms(1000);  // Update every second
    }
}